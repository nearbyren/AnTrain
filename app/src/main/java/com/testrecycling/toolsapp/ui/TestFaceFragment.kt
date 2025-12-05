package com.testrecycling.toolsapp.ui

import android.Manifest.permission.CAMERA
import android.annotation.SuppressLint
import android.content.Context.USB_SERVICE
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.ImageFormat
import android.graphics.Matrix
import android.graphics.Outline
import android.graphics.Rect
import android.graphics.YuvImage
import android.hardware.usb.UsbManager
import android.os.Bundle
import android.util.Log
import android.util.Size
import android.view.View
import android.view.ViewOutlineProvider
import android.widget.Toast
import androidx.annotation.OptIn
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.widget.AppCompatTextView
import androidx.camera.core.Camera
import androidx.camera.core.CameraSelector
import androidx.camera.core.ExperimentalLensFacing
import androidx.camera.core.ImageAnalysis
import androidx.camera.core.ImageProxy
import androidx.camera.core.Preview
import androidx.camera.core.resolutionselector.ResolutionSelector
import androidx.camera.core.resolutionselector.ResolutionStrategy
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.camera.view.PreviewView
import androidx.core.content.ContextCompat
import androidx.core.content.PermissionChecker
import androidx.core.view.isVisible
import androidx.fragment.app.viewModels
import androidx.lifecycle.lifecycleScope
import com.testrecycling.toolsapp.R
import com.testrecycling.toolsapp.databinding.FragmentTestFaceBinding
import com.testrecycling.toolsapp.fitsystembar.base.bind.BaseBindFragment
import com.testrecycling.toolsapp.utils.FaceEngineHelper
import com.testrecycling.toolsapp.utils.MediaPlayerHelper
import com.testrecycling.toolsapp.utils.PermissionRequest
import com.testrecycling.toolsapp.utils.PermissionUtils
import com.testrecycling.toolsapp.utils.PermissionsRequester
import com.testrecycling.toolsapp.view.FaceScanView
import com.seetaface.v6.FaceImagePreprocessor
import com.seetaface.v6.SeetaPointF
import com.seetaface.v6.config.FaceConfig
import com.testrecycling.toolsapp.utils.AppUtils
import com.testrecycling.toolsapp.utils.Loge
import com.testrecycling.toolsapp.vm.CabinetVM

import kotlinx.coroutines.launch
import java.io.ByteArrayOutputStream
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors

/***
 *检测人脸
 */
 class TestFaceFragment : BaseBindFragment<FragmentTestFaceBinding>() {
    private val cabinetVM: CabinetVM by viewModels(ownerProducer = { requireActivity() })
    private lateinit var previewView: PreviewView
    private var cameraProvider: ProcessCameraProvider? = null
    private var camerax141: Camera? = null
    private var imageAnalysis: ImageAnalysis? = null
    private var cameraSelector: CameraSelector? = null
    private var frameCallback: FrameDataCallback? = null
    private lateinit var cameraExecutor: ExecutorService

    //0.前置 1.后置 2.外接
    private var LENS_FACING_TYPE = 2
    private lateinit var permissionsManager: PermissionsRequester
    private var isGoPer = false

    //隐藏人脸示意图
    private var hideFaceSchematic = false

    companion object {
        const val REQUEST_CAMERA = 0
        const val REQUEST_STORAGE = 1


    }
    override fun isShowActionBar(): Boolean {
        return true
    }

    override fun isShowActionBarBack(): Boolean {
        return true
    }

    override fun doneCountdown() {
    }
    override fun layoutRes(): Int {
        return R.layout.fragment_test_face
    }

    override fun initialize(savedInstanceState: Bundle?) {
        cabinetVM.testMatchFaceQueue()
        setupUI()
        initCameraExecutor()
        initCameraX()
        lifecycleScope.launch {
            cabinetVM.faceTextMark.collect {
                binding.tvTips.text = "提示：${it}"
                binding.faceScan.refreshPrompt(it)
            }
        }
    }
    // 自定义数据回调接口
    interface FrameDataCallback {
        fun onFrameReceived(bitmap: Bitmap)
    }

    private fun initCameraExecutor() {
        cameraExecutor = Executors.newSingleThreadExecutor()
    }

    private fun setupUI() {
        binding.back.setOnClickListener { mActivity?.fragmentCoordinator?.navigateBack() }
        binding.tvCapture.setOnClickListener {
            cabinetVM.isTestAddDetection.value = false
            cabinetVM.isCaiQu.value = true
            cabinetVM.showDialogCollect.value = false
        }
        binding.tvFaceCount.text = "已录入人脸数(0)"
        cabinetVM.isBoxDetection.value = false
    }

    private fun initCameraX() {
        // 提前初始化视图
        previewView = binding.previewView.apply {
            implementationMode = PreviewView.ImplementationMode.PERFORMANCE
            scaleType = PreviewView.ScaleType.FILL_CENTER
        }
        startCameraxPermission()

//        setupCircularMask()
//        startLowLatencyPreview()
//        setupUsbCamera()
    }


    private fun startLowLatencyPreview() {
        // 步骤1：立即启动低分辨率预览
        val resolutionSelector =
                ResolutionSelector.Builder().setResolutionStrategy(ResolutionStrategy(Size(
                    FaceConfig.CAMERA_PREVIEW_WIDTH, FaceConfig.CAMERA_PREVIEW_HEIGHT), // 目标分辨率
                    ResolutionStrategy.FALLBACK_RULE_CLOSEST_HIGHER_THEN_LOWER)).build()

        val preview =
                Preview.Builder().setResolutionSelector(resolutionSelector) // 替代 setTargetResolution
                    .build().also {
                        it.surfaceProvider = previewView.surfaceProvider
                    }
        imageAnalysis =
                ImageAnalysis.Builder().setBackpressureStrategy(ImageAnalysis.STRATEGY_KEEP_ONLY_LATEST).setOutputImageFormat(ImageAnalysis.OUTPUT_IMAGE_FORMAT_RGBA_8888).build().also {
                    it.setAnalyzer(cameraExecutor) { proxy ->
                        if (!hideFaceSchematic) {
                            Log.e("TestFace", "网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 setAnalyzer ")
                            hideFaceSchematic = true
                            binding.faceScan.setShouldDrawOverlay(FaceScanView.CheckPreview.NONE)
                        }
                        processFrame(proxy)
                    }
                }
        // 步骤2：快速绑定摄像头
        val cameraProviderFuture = ProcessCameraProvider.getInstance(AppUtils.getContext())
//        val cameraProviderFuture = cabinetVM.cameraProviderFuture
//        TestApplication.cameraProviderFuture.addListener({
        cameraProviderFuture.addListener({
            try {
                cameraProvider = cameraProviderFuture.get()
                cameraSelector = findExternalCamera(LENS_FACING_TYPE)
                cameraProvider?.unbindAll()
                camerax141 = cameraSelector?.let {
                    cameraProvider?.bindToLifecycle(this, it, preview, imageAnalysis)
                }
                // 步骤3：延迟加载高分辨率配置
//                Handler(Looper.getMainLooper()).postDelayed({
//                    upgradePreviewQuality()
//                }, 1000)

            } catch (e: Exception) {
                Log.e("TestFace", "网络导入用户信息 Fast start failed: ${e.stackTraceToString()}")
            }
        }, ContextCompat.getMainExecutor(AppUtils.getContext()))
    }

    /***
     *  查找外接摄像头
     * @param selector
     *  0 前置摄像头头
     *  1 后置摄像头头
     *  2 外接摄像头头
     *  -1 未知摄像头
     */
    @OptIn(ExperimentalLensFacing::class) @SuppressLint("RestrictedApi")
    private fun findExternalCamera(selector: Int = CameraSelector.LENS_FACING_EXTERNAL): CameraSelector? {
        return cameraProvider?.availableCameraInfos?.firstOrNull { info ->

            val cameraSelector = when (selector) {
                CameraSelector.LENS_FACING_FRONT -> {
                    info.cameraSelector.lensFacing == CameraSelector.LENS_FACING_FRONT
                }

                CameraSelector.LENS_FACING_BACK -> {
                    info.cameraSelector.lensFacing == CameraSelector.LENS_FACING_BACK
                }

                CameraSelector.LENS_FACING_EXTERNAL -> {   // 通过特性判断外接摄像头
                    info.cameraSelector.lensFacing == CameraSelector.LENS_FACING_EXTERNAL
                }

                else -> {
                    info.cameraSelector.lensFacing == CameraSelector.LENS_FACING_UNKNOWN
                }
            }
            cameraSelector
        }?.cameraSelector
    }

    /** 暂停相机 */
    fun pauseCamera() {
        cameraProvider?.unbindAll()
        camerax141 = null
    }

    private fun startCameraxPermission() {
        if (PermissionUtils.hasSelfPermissions(AppUtils.getContext(), CAMERA)) {
            startLowLatencyPreview()
        } else {
            isGoPer = true
            initPermissions()
        }
    }

    /********************************************权限申请返回********************************************************/
    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<out String>, grantResults: IntArray) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        when (requestCode) {
            REQUEST_CAMERA -> {
                val hasCameraPermission = PermissionChecker.checkSelfPermission(AppUtils.getContext(), CAMERA)
                if (hasCameraPermission == PermissionChecker.PERMISSION_DENIED) {
                    Toast.makeText(AppUtils.getContext(), "您已拒绝权限访问。转到设置页面以打开权限", Toast.LENGTH_LONG).show()
                    return
                }
                if (isGoPer) {
                    startLowLatencyPreview()
                }
            }

            else -> {
            }
        }
    }

    /****************************************权限管理回调***************************************************/
    fun dealPermissionDenied() {
        Toast.makeText(AppUtils.getContext(), "某些权限被拒绝", Toast.LENGTH_LONG).show()
    }

    fun dealRequiresPermission() {
        Toast.makeText(AppUtils.getContext(), "授予的所有权限", Toast.LENGTH_LONG).show()
        if (isGoPer) {
            startLowLatencyPreview()
        }
    }

    fun dealShowRationale(request: PermissionRequest) {
        try {
            val builder = AlertDialog.Builder(AppUtils.getContext())
            val dialog = builder.create()
            val dialogView = View.inflate(AppUtils.getContext(), R.layout.dialog_permission, null)
            dialog.setView(dialogView)
            dialog.setCanceledOnTouchOutside(false)
            val btnCancel = dialogView.findViewById<AppCompatTextView>(R.id.cancel)
            val btnOK = dialogView.findViewById<AppCompatTextView>(R.id.ok)
            btnCancel.setOnClickListener { v: View? ->
                dialog.dismiss()
                request.cancel()
            }
            btnOK.setOnClickListener { v: View? ->
                dialog.dismiss()
                request.proceed()
            }
            dialog.setCanceledOnTouchOutside(false)
            dialog.show()
        } catch (e: Exception) {
            e.printStackTrace()
        }

    }

    fun dealNeverAskAgain() {
        Toast.makeText(AppUtils.getContext(), "有些许可被拒绝了，再也没有问过。", Toast.LENGTH_LONG).show()
//        val intent: Intent =
//                Intent(Settings.ACTION_APPLICATION_DETAILS_SETTINGS, Uri.fromParts("package", packageName, null))
//        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK)
//        startActivity(intent)
    }

    private fun initPermissions() {
        permissionsManager = mActivity?.let { PermissionsRequester(CAMERA, activity = it, onShowRationale = ::dealShowRationale, onPermissionDenied = ::dealPermissionDenied, onNeverAskAgain = ::dealNeverAskAgain, requiresPermission = ::dealRequiresPermission) }!!
        permissionsManager.launch()
    }

    /****************************************权限管理回调***************************************************/

    /** 处理帧数据 */
    private fun processFrame(imageProxy: ImageProxy) {
        if (!cabinetVM.isTestAddDetection.value) {
            //默认方式
            val bitmap = imageProxy.toBitmap()
            faceRecognition(bitmap)
        }
        cabinetVM.mainScope.launch {
            val size = cabinetVM.flowFaceCollect.value.size
            binding.tvFaceCount.text = "已录入人脸数(${size})"
        }
        imageProxy.close()
    }

    private fun faceRecognition(faceBitmap: Bitmap) {
        Loge.d("网络导入用户信息 FaceEngineHelper 人脸跟踪处理程序 faceRecognition ${cabinetVM.isCaiQu.value} | ${cabinetVM.flowFaceCollect.value.isEmpty()}")
        if (cabinetVM.isCaiQu.value) {
            val pointFs = arrayOfNulls<SeetaPointF>(5)
            val seetaImageData =
                    FaceImagePreprocessor.getInstance().getImageDataFromBitmap(faceBitmap)
            val faces = FaceEngineHelper.faceDetector?.detect(seetaImageData)
            faces?.let {
                cabinetVM.isTestAddDetection.value = true
                FaceEngineHelper.faceLandMarker?.mark(seetaImageData, faces[0]?.position, pointFs)
                val fint: Int = FaceEngineHelper.faceRecognizer?.extractFeatureSize ?: 1
                val feats = FloatArray(fint)
                //特征提取
                FaceEngineHelper.faceRecognizer?.extract(seetaImageData, pointFs, feats)
                cabinetVM.isCaiQu.value = false
                cabinetVM.showDialogCollect.value = true
                cabinetVM.mainScope.launch {
                    showConfirmDialog(faceBitmap, feats)
                }
            }?: run {
                cabinetVM.isTestAddDetection.value = false
                cabinetVM.mainScope.launch {
                    binding.tvTips.text = "未检测到人脸信息..."
                    binding.faceScan.refreshPrompt("未检测到人脸信息...")
                }
            }
            return
        }
         if (cabinetVM.flowFaceCollect.value.isEmpty()) {
            cabinetVM.isBoxDetection.value = false
            cabinetVM.mainScope.launch {
                binding.tvTips.text = "人脸识别库为空"
                binding.faceScan.refreshPrompt("人脸识别库为空")
            }
            MediaPlayerHelper.playAudio("shibienull")
            return
        }
        cabinetVM.testAddMatchFaceQueue(faceBitmap)
    }


    private fun showConfirmDialog(bitmap: Bitmap, feats2: FloatArray) {
        if (cabinetVM.showDialogCollect.value) {
            binding.clSelectContent.isVisible = true
            binding.preview.setImageBitmap(bitmap)
            binding.tvConfirm.setOnClickListener { _: View? ->
                binding.clSelectContent.isVisible = false
                cabinetVM.isTestAddDetection.value = false
                cabinetVM.isCaiQu.value = false
                cabinetVM.showDialogCollect.value = false
                cabinetVM.refreshFace(feats2)
            }
            binding.tvCancel.setOnClickListener { v: View? ->
                binding.clSelectContent.isVisible = false
                cabinetVM.isTestAddDetection.value = false
                cabinetVM.isCaiQu.value = false
                cabinetVM.showDialogCollect.value = false
            }
        }
    }

    override fun onDestroy() {
        pauseCamera()
        cabinetVM.testFaceQueue.cancel()
        super.onDestroy()
    }
    // ================= 扩展函数：Image转Bitmap =================
//    private fun ImageProxy.toBitmap(): Bitmap {
//        val buffer = planes[0].buffer
//        val bytes = ByteArray(buffer.remaining())
//        buffer.get(bytes)
//        return BitmapFactory.decodeByteArray(bytes, 0, bytes.size)
//    }

    // 将 ImageProxy 转换为 Bitmap 并自动旋转的扩展函数
    fun ImageProxy.toBitmap2(rotationDegrees: Int = 90): Bitmap {
        // 1. 获取 YUV 数据平面
        val planes = planes
        val yBuffer = planes[0].buffer
        val uBuffer = planes[1].buffer
        val vBuffer = planes[2].buffer

        // 2. 创建 YUV 到 RGB 的转换器
        val ySize = yBuffer.remaining()
        val uSize = uBuffer.remaining()
        val vSize = vBuffer.remaining()
        val nv21 = ByteArray(ySize + uSize + vSize)

        // 3. 将 YUV 数据转换为 NV21 格式
        yBuffer.get(nv21, 0, ySize)
        vBuffer.get(nv21, ySize, vSize)
        uBuffer.get(nv21, ySize + vSize, uSize)

        // 4. 使用 YuvImage 创建 Bitmap
        val yuvImage = YuvImage(nv21, ImageFormat.NV21, width, height, null)
        val outputStream = ByteArrayOutputStream()
        yuvImage.compressToJpeg(Rect(0, 0, width, height), 100, outputStream)
        val imageBytes = outputStream.toByteArray()
        val originalBitmap = BitmapFactory.decodeByteArray(imageBytes, 0, imageBytes.size)

        // 5. 执行旋转操作
        val matrix = Matrix().apply {
            postRotate(rotationDegrees.toFloat())
            postScale(-1f, 1f) // 如果需要镜像翻转（前置摄像头）
        }

        return Bitmap.createBitmap(originalBitmap, 0, 0, originalBitmap.width, originalBitmap.height, matrix, true)
    }

    // ================= 数据回调设置 =================
    fun setFrameDataCallback(callback: FrameDataCallback) {
     }

    // 圆形裁剪设置
    private fun setupCircularMask() {
        // 设置圆角裁剪（仅 TextureView 支持）
        if (previewView.implementationMode == PreviewView.ImplementationMode.PERFORMANCE) {
            previewView.post {
                previewView.outlineProvider = object : ViewOutlineProvider() {
                    override fun getOutline(view: View, outline: Outline) {
//                        outline.setOval(0, 0, view.width, view.height)
//                        outline.setRoundRect(0, 0, view.width, view.height, view.height / 2f)
                        val size = view.width.coerceAtMost(view.height) // 取宽高中的较小值
                        val left = (view.width - size) / 2
                        val top = (view.height - size) / 2

                        // 方法1：直接绘制圆形（API 21+）
                        outline.setOval(left, top, left + size, top + size)
                    }
                }
                previewView.clipToOutline = true
                Log.e("TestFace", "网络导入用户信息 outlineProvider")
            }
        }
    }

    // ================= 核心相机逻辑 =================
    private fun setupUsbCamera() {
        val cameraProviderFuture = ProcessCameraProvider.getInstance(AppUtils.getContext())
        cameraProviderFuture.addListener({
            Log.e("TestFace", "网络导入用户信息 setupUsbCamera cameraProviderFuture addListener")
            cameraProvider = cameraProviderFuture.get()
            checkUsbCameraAvailability()
        }, ContextCompat.getMainExecutor(AppUtils.getContext()))
    }

    /** 检测外接USB摄像头 */
    private fun checkUsbCameraAvailability() {
        val usbManager = mActivity?.getSystemService(USB_SERVICE) as UsbManager
        if (usbManager.deviceList.values.isEmpty()) {
            return
        }
        startLowLatencyPreview()
    }

    private fun upgradePreviewQuality() {
        // 升级到高清预览并启动分析

        val resolutionSelector =
                ResolutionSelector.Builder().setResolutionStrategy(ResolutionStrategy(Size(FaceConfig.CAMERA_PREVIEW_WIDTH, FaceConfig.CAMERA_PREVIEW_HEIGHT), // 目标分辨率
                    ResolutionStrategy.FALLBACK_RULE_CLOSEST_HIGHER_THEN_LOWER)).build()

        val preview =
                Preview.Builder().setResolutionSelector(resolutionSelector) // 替代 setTargetResolution
                    .build().also {
                        it.surfaceProvider = previewView.surfaceProvider
                    }
        imageAnalysis =
                ImageAnalysis.Builder().setBackpressureStrategy(ImageAnalysis.STRATEGY_KEEP_ONLY_LATEST).setOutputImageFormat(ImageAnalysis.OUTPUT_IMAGE_FORMAT_RGBA_8888).build().also {
                    it.setAnalyzer(cameraExecutor) { proxy ->
                        processFrame(proxy)
                    }
                }

        cameraSelector?.let {
            cameraProvider?.bindToLifecycle(this, it, preview, imageAnalysis)
        }
    }
}