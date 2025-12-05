package com.testrecycling.toolsapp.utils

import android.content.Context
import com.seetaface.v6.FaceAntiSpoofing
import com.seetaface.v6.FaceDetector
import com.seetaface.v6.FaceEngine
import com.seetaface.v6.FaceLandMarker
import com.seetaface.v6.FaceRecognizer
import com.seetaface.v6.SeetaUtils
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch

object FaceEngineHelper {

    private var faceEngine: FaceEngine? = null
    val faceDetector: FaceDetector?
        get() = faceEngine?.faceDetector
    val faceLandMarker: FaceLandMarker?
        get() = faceEngine?.faceLandMarker
    val faceRecognizer: FaceRecognizer?
        get() = faceEngine?.faceRecognizer
    val faceAntiSpoofing: FaceAntiSpoofing?
        get() = faceEngine?.faceAntiSpoofing

    private val ioScope = CoroutineScope(Dispatchers.IO)

    @Volatile
    var isInitialized = false

    /**
     * 初始化人脸引擎，确保只调用一次。
     */
    fun initialize(context: Context, callback: (() -> Unit)? = null) {
        if (isInitialized) {
            callback?.invoke()
            return
        }

        synchronized(this) {
            if (!isInitialized) {
                ioScope.launch {
                    if (SeetaUtils.shouldCopyAssetsModels(context)) {
                        Loge.d("网络导入用户信息 FaceEngineHelper: 模型文件不存在，正在复制")
                        SeetaUtils.copyModelsToExternalStorage(context)
                    } else {
                        Loge.d("网络导入用户信息 FaceEngineHelper: 模型文件已存在")
                    }

                    delay(500) // 确保文件操作完成
                    faceEngine = FaceEngine.getInstance()
                    isInitialized = true
                    Loge.d("网络导入用户信息 FaceEngineHelper: 人脸引擎初始化完成")
                    callback?.invoke()
                }
            }
        }
    }
}
