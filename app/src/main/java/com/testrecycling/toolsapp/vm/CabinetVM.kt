package com.testrecycling.toolsapp.vm

import android.graphics.Bitmap
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.lifecycle.ViewModel
import com.google.common.util.concurrent.ListenableFuture
import com.seetaface.v6.FaceImagePreprocessor
import com.seetaface.v6.SeetaPointF
import com.seetaface.v6.TrackingInfo
import com.seetaface.v6.config.FaceConfig
import com.seetaface.v6.enums.EnumFaceAntiStatus
import com.testrecycling.toolsapp.utils.AppUtils
import com.testrecycling.toolsapp.utils.FaceEngineHelper
import com.testrecycling.toolsapp.utils.Loge
import com.testrecycling.toolsapp.utils.MediaPlayerHelper
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.MainScope
import kotlinx.coroutines.SupervisorJob
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asSharedFlow
import kotlinx.coroutines.isActive
import kotlinx.coroutines.launch
import javax.inject.Inject


@HiltViewModel
class CabinetVM @Inject constructor() : ViewModel() {


    /**
     * dbIoJob
     */
    private var dbIoJob: Job? = null

    /**
     * 用于处理 I/O 操作的协程作用域
     */
    val ioScope = CoroutineScope(SupervisorJob() + Dispatchers.IO)

    /**
     * 用于处理 main 操作的协程作用域
     */
    val mainScope = MainScope()

    /***
     * 创建一个Channel，类型为Int，表示命令类型 232串口
     */
    private val statusQueue = Channel<Int>()

    /***
     * 创建一个Channel，类型为Int，表示命令类型
     */
    private val doorQueue = Channel<Int>()
    private val doorQueue2 = Channel<Int>(capacity = 3)

    /***
     * 创建一个Channel，类型为Int，表示命令类型 485串口
     */
    private val commandQueue485 = Channel<Int>()

    private val sendFileByte232 = Channel<ByteArray>()

    private val sendFileByte485 = Channel<ByteArray>()

    private val faceQueue = Channel<Bitmap>()

    private val markQueue = Channel<Boolean>()

    /***
     * 创建一个Channel，类型为Int，表示命令类型
     */
    private val masterChipQueue = Channel<Int>()

    /***
     * 创建一个Channel，类型为Int，表示命令类型
     */
    private val fromChipQueue = Channel<Int>()

    /***
     * 创建一个Channel，类型为Int，表示命令类型
     */
    private val queue485 = Channel<Int>()

    /***
     * 用于处理 默认协程作用域
     */
    val defaultScope = CoroutineScope(SupervisorJob() + Dispatchers.Default)

    ///领取箱子检测中
    private val flowBoxDetection = MutableStateFlow(false)
    val isBoxDetection: MutableStateFlow<Boolean> = flowBoxDetection

    //相机提供者
    var cameraProviderFuture: ListenableFuture<ProcessCameraProvider>? = null

    /*******************************************测试工装模块*************************************************/

    ///防止点击录入多次
    private val flowCaiQu = MutableStateFlow(false)
    val isCaiQu: MutableStateFlow<Boolean> = flowCaiQu
    private val flowFaceTextMark = MutableSharedFlow<String>(replay = 0)
    val faceTextMark = flowFaceTextMark.asSharedFlow()

    //添加人脸检测中
    private val flowTestAddDetection = MutableStateFlow(true)
    val isTestAddDetection: MutableStateFlow<Boolean> = flowTestAddDetection

    //标记是否弹框了
    private val flowShowDialog = MutableStateFlow(true)
    val showDialogCollect: MutableStateFlow<Boolean> = flowShowDialog

    ///维护未识别到人脸识别次数
    val flowNoFaceCount = MutableStateFlow(0)
    val getNoFaceCount: MutableStateFlow<Int> = flowNoFaceCount

    //只有数据不同才会更新面部
    private val flowAddFace = MutableStateFlow<MutableList<FloatArray>>(mutableListOf())
    val flowFaceCollect: StateFlow<MutableList<FloatArray>> = flowAddFace
    fun refreshFace(feat: FloatArray) {
        Loge.d("测试刷新数据 updateFaceFeats")
        val list = flowAddFace.value
        list.add(feat)
        flowAddFace.value = list
    }

    /**
     * 人脸相似度比对
     *
     * @param fd1
     * @param fd2
     * @return 相似度达到0.8 则 true
     */
    private fun faceCompare(fd1: FloatArray, fd2: FloatArray): Boolean {
        val sim = FaceEngineHelper.faceRecognizer?.compareFace(fd1, fd2) ?: 0.0f
        Loge.d("网络导入用户信息 face--- 人脸跟踪处理程序 特征检测和提取 开启识别 sim $sim ${Thread.currentThread().name}")
        return if (sim >= FaceConfig.FACE_SIMILARITY_DEGREE) {
            true
        } else {
            false
        }
    }

    fun testSearchFaceLocal(faceFeature: FloatArray): Boolean {
        val list = flowAddFace.value
        if (list.isNotEmpty()) {
            list.let {
                for (user in list) {
                    return (faceCompare(faceFeature, user))
                }
            }
        }
        return false
    }

    val testFaceQueue = Channel<Bitmap>(Channel.CONFLATED)

    /***
     * 添加人脸识别队列
     */
    fun testAddMatchFaceQueue(bitmap: Bitmap) {
        ioScope.launch {
            testFaceQueue.send(bitmap)
        }
    }

    /***
     * 执行人脸识别队列
     */
    fun testMatchFaceQueue() {
        defaultScope.launch {
            while (isActive) {
                val bitmap = testFaceQueue.receive()
                Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 开始匹配 | ${isBoxDetection.value} | ${Thread.currentThread().name}")
                bitmap.let { faceBitmap ->
//                    antiStatusYes(faceBitmap)
//                    antiStatusNo(faceBitmap)
//                    testAntiStatusYes2(faceBitmap)
                    testAntiStatusNo(faceBitmap)

                }
            }
        }
    }

    fun emitText(text: String) {
        ioScope.launch {
            flowFaceTextMark.emit(text)
        }
    }

    /***
     * 活体检测
     */
    fun antiStatusYes(faceBitmap: Bitmap) {
        Loge.d("网络导入用户信息 获取人脸信息组开始")
        val pointFs = arrayOfNulls<SeetaPointF>(5)
        val seetaImageData = FaceImagePreprocessor.getInstance().getImageDataFromBitmap(faceBitmap)
        val faces = FaceEngineHelper.faceDetector?.detect(seetaImageData)
        faces?.let {
            Loge.d("网络导入用户信息 提取人脸信息数组了")
            isBoxDetection.value = true
            FaceEngineHelper.faceLandMarker?.mark(seetaImageData, faces[0]?.position, pointFs)
            Loge.d("网络导入用户信息 获取的人脸特征点数组")
            val fint: Int = FaceEngineHelper.faceRecognizer?.extractFeatureSize ?: 1
            val feats = FloatArray(fint)
            //特征提取
            FaceEngineHelper.faceRecognizer?.extract(seetaImageData, pointFs, feats)
            Loge.d("网络导入用户信息 返回的人脸特征值数组")
            //活体检测
            var maxIndex = 0
            var maxWidth = 0.0
            for (i in faces.indices) {
                if (faces[i].position.width > maxWidth) {
                    maxIndex = i
                    maxWidth = faces[i].position.width.toDouble()
                }
            }
            val trackingInfo = TrackingInfo().apply {
                faceInfo = faces[maxIndex].position
                faceInfo.x = faces[maxIndex].position.x
                faceInfo.y = faces[maxIndex].position.y
                faceInfo.width = faces[maxIndex].position.width
                faceInfo.height = faces[maxIndex].position.height
            }
            val predictStatus = FaceEngineHelper.faceAntiSpoofing?.predict(
                seetaImageData, trackingInfo.faceInfo, pointFs
            )
            Loge.d("网络导入用户信息 获取活体检测了")
            val faceAntiStatus = EnumFaceAntiStatus.getFaceAntiStatus(predictStatus)
            Loge.d("网络导入用户信息 匹配活体结束了")
            faceAntiStatus?.let { anti ->
                isBoxDetection.value = true
                if (EnumFaceAntiStatus.STATUS_REAL != anti) {
                    Loge.d("网络导入用户信息 假人....")
                    MediaPlayerHelper.playAudio("jiance")//人脸识别失败
                    emitText("人脸识别失败 ${getNoFaceCount.value}")
                } else {
                    val chenggong = testSearchFaceLocal(feats)
                    if (chenggong) {
                        flowNoFaceCount.value++
                        Loge.d("网络导入用户信息 识别成功")
                        MediaPlayerHelper.playAudio("chenggong")//人脸识别成功
                        emitText("人脸匹配成功 ${getNoFaceCount.value}")
                    } else {
                        Loge.d("网络导入用户信息 失败了")
                        MediaPlayerHelper.playAudio("jiance")//人脸识别失败
                        emitText("请人脸对准识别区域内进行识别 ${getNoFaceCount.value}")
                    }
                }
            }
        } ?: run {
            MediaPlayerHelper.playAudio("weijiance")//人脸识别失败
            isBoxDetection.value = false
            Loge.d("网络导入用户信息 没有获取到人脸信息组 faces null")
        }
        Loge.d("网络导入用户信息 获取人脸信息组结束")
    }

    /***
     * 活体检测 裁剪
     */
    fun testAntiStatusYes2(faceBitmap: Bitmap) {
        Loge.d("网络导入用户信息 获取人脸信息组开始")
        val pointFs = arrayOfNulls<SeetaPointF>(5)
        val seetaImageData = FaceImagePreprocessor.getInstance().getImageDataFromBitmap(faceBitmap)
        val faces = FaceEngineHelper.faceDetector?.detect(seetaImageData)
        faces?.let {
            Loge.d("网络导入用户信息 提取人脸信息数组了")
            isBoxDetection.value = true
            FaceEngineHelper.faceLandMarker?.mark(seetaImageData, faces[0]?.position, pointFs)
            Loge.d("网络导入用户信息 获取的人脸特征点数组")
            val predictStatus = FaceEngineHelper.faceAntiSpoofing?.predict(
                seetaImageData, faces[0].position, pointFs
            )
            val faceAntiStatus = EnumFaceAntiStatus.getFaceAntiStatus(predictStatus)
            Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 匹配活体")
            faceAntiStatus?.let { anti ->
                isBoxDetection.value = true
                if (EnumFaceAntiStatus.STATUS_REAL != anti) {
                    Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 假人....")
                    MediaPlayerHelper.playAudio("jiance")//人脸识别失败
                    emitText("人脸识别失败 ${getNoFaceCount.value}")
                } else {
                    Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 开始匹配人脸")
                    val seetaImageData2 =
                        FaceEngineHelper.faceRecognizer?.cropFaceV2(seetaImageData, pointFs)
                    val fint2: Int = FaceEngineHelper.faceRecognizer?.extractFeatureSize ?: 1
                    val feats2 = FloatArray(fint2)
                    FaceEngineHelper.faceRecognizer?.extractCropFace(seetaImageData2, feats2)
                    Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 开始匹配人脸")
                    val chenggong = testSearchFaceLocal(feats2)
                    if (chenggong) {
                        flowNoFaceCount.value++
                        Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 图片匹配 耗时 结束 识别成功")
                        MediaPlayerHelper.playAudio("chenggong")//人脸识别成功
                        emitText("人脸匹配成功 ${getNoFaceCount.value}")
                    } else {
                        Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 图片匹配 特征检测和提取 开启识别 失败了")
                        MediaPlayerHelper.playAudio("jiance")//人脸识别失败
                        emitText("请人脸对准识别区域内进行识别 ${getNoFaceCount.value}")
                    }
                }
            }
        } ?: run {
            Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 图片匹配 没有获取到人脸信息组 faces null")
            MediaPlayerHelper.playAudio("weijiance")//人脸识别失败
            emitText("未检测到人脸信息...${getNoFaceCount.value}")
        }
        Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 获取人脸信息组结束")
    }

    /**
     *不活体检测
     */
    fun antiStatusNo(faceBitmap: Bitmap) {
        val pointFs = arrayOfNulls<SeetaPointF>(5)
        val seetaImageData = FaceImagePreprocessor.getInstance().getImageDataFromBitmap(faceBitmap)
        val faces = FaceEngineHelper.faceDetector?.detect(seetaImageData)
        faces?.let {
            Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 获取人脸信息组开始")
            val seetaImageData2 =
                FaceEngineHelper.faceRecognizer?.cropFaceV2(seetaImageData, pointFs)
            val fint2: Int = FaceEngineHelper.faceRecognizer?.extractFeatureSize ?: 1
            val feats2 = FloatArray(fint2)
            FaceEngineHelper.faceRecognizer?.extractCropFace(seetaImageData2, feats2)
            Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 开始匹配人脸")
            val chenggong = testSearchFaceLocal(feats2)
            if (chenggong) {
                flowNoFaceCount.value++
                Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 图片匹配 耗时 结束 识别成功")
                MediaPlayerHelper.playAudio("chenggong")//人脸识别成功
                emitText("人脸匹配成功 ${getNoFaceCount.value}")
            } else {
                Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 图片匹配 特征检测和提取 开启识别 失败了")
                MediaPlayerHelper.playAudio("jiance")//人脸识别失败
                emitText("请人脸对准识别区域内进行识别 ${getNoFaceCount.value}")
            }

        } ?: run {
            MediaPlayerHelper.playAudio("weijiance")//人脸识别失败
            isBoxDetection.value = false
        }
        Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 获取人脸信息组结束")

    }

    /**
     *不活体检测 裁剪
     */
    fun testAntiStatusNo(faceBitmap: Bitmap) {
        val pointFs = arrayOfNulls<SeetaPointF>(5)
        val seetaImageData = FaceImagePreprocessor.getInstance().getImageDataFromBitmap(faceBitmap)
        val faces = FaceEngineHelper.faceDetector?.detect(seetaImageData)
        faces?.let {
            Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 获取人脸信息组开始")
            isBoxDetection.value = true
            FaceEngineHelper.faceLandMarker?.mark(seetaImageData, faces[0]?.position, pointFs)
            Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 匹配活体")
            Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 开始匹配人脸")
            val seetaImageData2 =
                FaceEngineHelper.faceRecognizer?.cropFaceV2(seetaImageData, pointFs)
            val fint2: Int = FaceEngineHelper.faceRecognizer?.extractFeatureSize ?: 1
            val feats2 = FloatArray(fint2)
            FaceEngineHelper.faceRecognizer?.extractCropFace(seetaImageData2, feats2)
            Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 开始匹配人脸")
            val chenggong = testSearchFaceLocal(feats2)
            if (chenggong) {
                flowNoFaceCount.value++
                Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 图片匹配 耗时 结束 识别成功")
                MediaPlayerHelper.playAudio("chenggong")//人脸识别成功
                emitText("人脸匹配成功 ${getNoFaceCount.value}")
            } else {
                Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 图片匹配 特征检测和提取 开启识别 失败了")
                MediaPlayerHelper.playAudio("jiance")//人脸识别失败
                emitText("请人脸对准识别区域内进行识别 ${getNoFaceCount.value}")
            }
        } ?: run {
            MediaPlayerHelper.playAudio("weijiance")//人脸识别失败
            isBoxDetection.value = false
        }
        Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 获取人脸信息组结束")

    }

    /*******************************************测试工装模块*************************************************/

}