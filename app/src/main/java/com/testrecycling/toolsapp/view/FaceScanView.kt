package com.testrecycling.toolsapp.view


import android.animation.Animator
import android.animation.AnimatorListenerAdapter
import android.animation.ValueAnimator
import android.annotation.SuppressLint
import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.LinearGradient
import android.graphics.Paint
import android.graphics.Path
import android.graphics.RectF
import android.graphics.Shader
import android.util.AttributeSet
import android.view.View
import android.view.animation.LinearInterpolator
import com.testrecycling.toolsapp.R
import kotlin.math.min

/***
 * 面部人脸扫码
 */
class FaceScanView @JvmOverloads constructor(context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = 0) : View(context, attrs, defStyleAttr) {

    /***
     * 扩展函数：dp转px
     */
    private fun Float.dpToPx(): Float = this * resources.displayMetrics.density

    /***
     * 扩展函数：sp转px（如果未定义，可添加）
     */
    private fun Float.spToPx(): Float = this * resources.displayMetrics.density

    /***
     * 全屏幕背景图
     */
    private val backgroundBitmap: Bitmap? = try {
        BitmapFactory.decodeResource(resources, R.drawable.ic_bg_content_small)
    } catch (e: Exception) {
        null
    }

    /***
     * 绘制全屏时使用的画笔
     */
    private val backgroundPaint = Paint().apply {
        isAntiAlias = true
    }

    /***
     * 椭圆边框参数
     */
    private val ovalRect = RectF()

    /***
     * 首次预览
     */
    private var firstPreview = true

    /***
     * 圆形参数
     */
    private var circleRadius = 0f

    /***
     * 绘制外部圆形的画笔
     */
    @SuppressLint("ResourceAsColor")
    private val externalCirclePaint = Paint().apply {
        color = Color.parseColor("#FF35B8FE")
        style = Paint.Style.STROKE
        strokeWidth = 10f.dpToPx() // 边框宽度
        isAntiAlias = true
    }

    /***
     * 绘制内部圆形的画笔
     */
    private val internalCirclePaint = Paint().apply {
        color = Color.parseColor("#A8245690")
        style = Paint.Style.FILL
        isAntiAlias = true
    }

    /***
     *  绘制标记（打勾/打叉）的画笔
     */
    private val markPaint = Paint().apply {
        color = Color.WHITE
        style = Paint.Style.STROKE
        strokeWidth = 6f.dpToPx()
        isAntiAlias = true
        strokeCap = Paint.Cap.ROUND // 设置线条末端为圆形
    }

    /***
     * 标记路径
     */
    private val markPath = Path()

    /***
     * 动画进度
     */
    private var markProgress = 0f

    /***
     * 打叉打勾动画控制器
     */
    private val markAnimator = ValueAnimator.ofFloat(0f, 1f).apply {
//        duration = 200
        interpolator = LinearInterpolator()
        addUpdateListener { animation ->
            markProgress = animation.animatedValue as Float
            invalidate()
        }
        addListener(object : AnimatorListenerAdapter() {
            override fun onAnimationEnd(animation: Animator) {
                super.onAnimationEnd(animation)
//                faceScanCompleteListener?.onFaceComplete(if (currentState == CheckState.CHECKED) true else if (currentState == CheckState.CROSS) false else false)
                faceScanCompleteListener?.onFaceComplete2(if (currentState == CheckState.CHECKED) CheckState.CHECKED else if (currentState == CheckState.CROSS) CheckState.CROSS else CheckState.NONE)
            }
        })
    }

    /***
     * 扫描线动画控制器
     */
    private val scanAnimator = ValueAnimator.ofFloat(0f, 1f).apply {
        // 加快扫描速度
        duration = 3000
        repeatCount = ValueAnimator.INFINITE
        interpolator = LinearInterpolator()
        addUpdateListener {
            scanProgress = it.animatedValue as Float
            invalidate()
        }
        addListener(object : AnimatorListenerAdapter() {
            override fun onAnimationRepeat(animation: Animator) {
                super.onAnimationRepeat(animation)
                faceScanCompleteListener?.onScanComplete()
            }
        })
    }

    /***
     * 预生成椭圆路径（提升性能）
     */
    private val ovalPath by lazy {
        Path().apply { addOval(ovalRect, Path.Direction.CW) }
    }

    /***
     * 绘制椭圆边框的画笔
     */
    private val borderPaint = Paint().apply {
        style = Paint.Style.STROKE
        strokeWidth = 6f.dpToPx()  // 适配屏幕密度
        color = resources.getColor(R.color.face_border)
        isAntiAlias = true
    }

    /***
     * 加粗扫描线
     */
    private val scanHeight = 50f.dpToPx()

    /***
     * 扫描线动画进度
     */
    private var scanProgress = 0f

    /***
     * 扫描渐变色
     */
    private var scanGradient: LinearGradient? = null

    /***
     * 扫描线参数（增强版）
     */
    private val scanPaint = Paint().apply {
        style = Paint.Style.FILL
        isAntiAlias = true
    }

    // ========== 新增回调接口 ==========
    interface OnFaceScanCompleteListener {
        fun onScanComplete()
        fun onFaceComplete(success: Boolean)
        fun onFaceComplete2(state: CheckState)
    }

    private var faceScanCompleteListener: OnFaceScanCompleteListener? = null

    fun setOnScanCompleteListener(listener: OnFaceScanCompleteListener) {
        this.faceScanCompleteListener = listener
    }

    /***
     * 绘制背景图
     */
    private val overlayPaint = Paint().apply {
        isAntiAlias = true
//        alpha = 220 // 默认半透明效果
//        alpha = 0 // 默认半透明效果
    }

    /***
     * 绘制预览人脸信息
     */
    private var overlayBitmap: Bitmap? = try {
//        val bitmp = BitmapFactory.decodeResource(resources, R.drawable.gggg)
//        val scale = calculateOvalScale(bitmp, ovalRect.width(), ovalRect.height())
//   val b =      Bitmap.createScaledBitmap(
//            bitmp,
//            (bitmp.width * scale).toInt(),
//            (bitmp.height * scale).toInt(),
//            true
//        )
//        b
//        BitmapFactory.decodeResource(resources, R.drawable.ic_rentou)
        BitmapFactory.decodeResource(resources, R.drawable.ic_rentou2)
    } catch (e: Exception) {
        null
    }

    private var overlayBitmapNull: Bitmap? = try {
//        val bitmp = BitmapFactory.decodeResource(resources, R.drawable.gggg)
//        val scale = calculateOvalScale(bitmp, ovalRect.width(), ovalRect.height())
//   val b =      Bitmap.createScaledBitmap(
//            bitmp,
//            (bitmp.width * scale).toInt(),
//            (bitmp.height * scale).toInt(),
//            true
//        )
//        b
//        BitmapFactory.decodeResource(resources, R.drawable.ic_rentou)
        BitmapFactory.decodeResource(resources, R.color.transparent)
    } catch (e: Exception) {
        null
    }

    /***
     * 椭圆底部到文字的间距
     */
    private val textMarginBottom = 100f.dpToPx()

    private var mPromptText = "正在初始化人脸库... 请稍等..."

    /***
     * 文字参数
     */
    private val promptTextPaint = Paint().apply {
        color = Color.WHITE
        textSize = 32f.spToPx()
        textAlign = Paint.Align.CENTER
    }

    /**
     * 在椭圆中心绘制圆形及带动画的标记（打勾/打叉）
     */
    private fun drawCircleWithAnimatedMark(canvas: Canvas) {
        // 计算圆形的中心点
        val centerX = ovalRect.centerX()
        val centerY = ovalRect.centerY()

        // 使用半透明蓝色填充圆形内部（缩小半径以适应边框内部）// 减去边框宽度的一半
        canvas.drawCircle(centerX, centerY, circleRadius - externalCirclePaint.strokeWidth / 2, internalCirclePaint)
        // 绘制圆形边框（蓝色，宽度10dp）
        canvas.drawCircle(centerX, centerY, circleRadius, externalCirclePaint)
        // 绘制带动画的标记（打勾或打叉）
        if (markProgress > 0f) {
            markPath.reset()
            when (currentState) {
                CheckState.CHECKED -> drawCheckMark(canvas, centerX, centerY, circleRadius)
                CheckState.CROSS -> drawCrossMark(canvas, centerX, centerY, circleRadius)
                CheckState.NONE -> Unit // 不做操作
            }
        }
    }

    /**
     * 绘制带动画的打勾
     */
    private fun drawCheckMark(canvas: Canvas, centerX: Float, centerY: Float, radius: Float) {
        val progress = markProgress

        // 打勾的起点、拐点和终点
        val startX = centerX - radius * 0.4f
        val startY = centerY
        val midX = centerX - radius * 0.1f
        val midY = centerY + radius * 0.4f
        val endX = centerX + radius * 0.4f
        val endY = centerY - radius * 0.4f

        // 绘制第一段线
        markPath.moveTo(startX, startY)
        markPath.lineTo(startX + (midX - startX) * progress, startY + (midY - startY) * progress)

        // 如果进度超过一半，绘制第二段线
        if (progress > 0.5f) {
            val secondProgress = (progress - 0.5f) / 0.5f
            markPath.lineTo(midX + (endX - midX) * secondProgress, midY + (endY - midY) * secondProgress)
        }

        canvas.drawPath(markPath, markPaint)
    }

    /**
     * 绘制带动画的打叉
     */
    private fun drawCrossMark(canvas: Canvas, centerX: Float, centerY: Float, radius: Float) {
        val progress = markProgress

        // 左上到右下的斜线
        val startX1 = centerX - radius * 0.4f
        val startY1 = centerY - radius * 0.4f
        val endX1 = centerX + radius * 0.4f
        val endY1 = centerY + radius * 0.4f

        // 右上到左下的斜线
        val startX2 = centerX + radius * 0.4f
        val startY2 = centerY - radius * 0.4f
        val endX2 = centerX - radius * 0.4f
        val endY2 = centerY + radius * 0.4f

        // 绘制第一段斜线
        markPath.moveTo(startX1, startY1)
        markPath.lineTo(startX1 + (endX1 - startX1) * progress, startY1 + (endY1 - startY1) * progress)

        // 如果进度超过一半，绘制第二段斜线
        if (progress > 0.5f) {
            val secondProgress = (progress - 0.5f) / 0.5f
            markPath.moveTo(startX2, startY2)
            markPath.lineTo(startX2 + (endX2 - startX2) * secondProgress, startY2 + (endY2 - startY2) * secondProgress)
        }

        canvas.drawPath(markPath, markPaint)
    }

    // 动画控制
    private var currentState = CheckState.NONE
    private var currentPreview = CheckPreview.Preview

    // 状态枚举
    enum class CheckState {
        NONE, CHECKED, CROSS
    }


    enum class CheckPreview {
        NONE, Preview
    }

    /**
     * 设置标记类型并启动动画
     */
    fun setMark(state: CheckState = CheckState.NONE) {
        this.currentState = state
        markProgress = 0f
        if (!markAnimator.isRunning) {
            markAnimator.start()
        }
//        invalidate()
    }

    ///绘制扫描线
    private fun drawScanLine(canvas: Canvas) {
//        // 2. 双重区域限制（确保扫描线不溢出）
//        val saveCount = canvas.saveLayer(ovalRect, null)
//        canvas.clipPath(ovalPath)
//
//        // 3. 绘制增强版扫描线
//        val scanY = ovalRect.top + (ovalRect.height() * scanProgress)
//        canvas.drawRect(ovalRect.left, scanY - scanHeight, ovalRect.right, scanY, scanPaint)
//
//        // 4. 添加扫描线光晕效果
//        scanPaint.color = Color.parseColor("#807DD8F2")
//        canvas.drawRect(ovalRect.left, scanY - scanHeight / 1.1f, ovalRect.right, scanY + scanHeight / 5.8f, scanPaint)
//
//        canvas.restore()
        val saveCount = canvas.saveLayer(ovalRect, null)
        canvas.clipPath(ovalPath)

        // 计算扫描线位置（带偏移量让效果更自然）
        val scanY = ovalRect.top + (ovalRect.height() * scanProgress) - scanHeight / 2

        // 绘制光晕效果（先绘制）
        glowBitmap?.let {
            val glowOffset = scanHeight * 0.6f
            canvas.drawBitmap(it, ovalRect.left, scanY - glowOffset, scanPaint.apply {
                alpha = 120
            })
        }

        // 绘制核心扫描线
        scanLineBitmap?.let {
            canvas.drawBitmap(it, ovalRect.left, scanY, scanPaint.apply { alpha = 200 })
        }

        canvas.restoreToCount(saveCount)
    }

    ///绘制提示语
    private fun drawPromptText(canvas: Canvas) {
        //确保对齐方式已设置
        promptTextPaint.textAlign = Paint.Align.CENTER
        //水平中心坐标
        val textX = width / 2f
        //基线位置
        val textBaselineY = ovalRect.bottom + textMarginBottom + promptTextPaint.textSize
        canvas.drawText(mPromptText, textX, textBaselineY, promptTextPaint)
    }

    ///绘制椭圆预览图
    private fun drawClippedBitmapPreview(canvas: Canvas, bitmap: Bitmap?, paint: Paint) {
        bitmap?.let { bmp ->
            // 关键修改1：基于椭圆区域计算缩放比例
            val ovalWidth = ovalRect.width()
            val ovalHeight = ovalRect.height()

            // 计算适应椭圆区域的缩放比例
            val scale = calculateOvalScale(bmp, ovalWidth, ovalHeight)

            // 关键修改2：计算在椭圆区域内的居中位置
            val scaledWidth = bmp.width * scale
            val scaledHeight = bmp.height * scale
            val left = ovalRect.left + (ovalWidth - scaledWidth) / 2
            val top = ovalRect.top + (ovalHeight - scaledHeight) / 2

            canvas.save()
            // 使用椭圆剪裁
            canvas.clipPath(Path().apply { addOval(ovalRect, Path.Direction.CW) })
            // 直接定位到椭圆区域绘制
            canvas.translate(left, top)
            canvas.scale(scale, scale)
            canvas.drawBitmap(bmp, 0f, 0f, paint)
            canvas.restore()
        }
    }

    // 新增椭圆区域适配的缩放计算
    private fun calculateOvalScale(bitmap: Bitmap, ovalWidth: Float, ovalHeight: Float): Float {
        val widthRatio = ovalWidth / bitmap.width
        val heightRatio = ovalHeight / bitmap.height
        return min(widthRatio, heightRatio) // 保持图片完整显示在椭圆内
    }

    // 绘制全屏背景图
    private fun drawClippedBitmapBackground(canvas: Canvas, bitmap: Bitmap?, paint: Paint) {
        bitmap?.let { bmp ->
            val scale = calculateBitmapScale(bmp)
            val (left, top) = calculateBitmapPosition(bmp, scale)

            val path = createClippingPath()

            canvas.save()
            canvas.clipPath(path)
            canvas.translate(left, top)
            canvas.scale(scale, scale)
            canvas.drawBitmap(bmp, 0f, 0f, paint)
            canvas.restore()
        }
    }

    // 计算缩放比例
    private fun calculateBitmapScale(bitmap: Bitmap): Float {
        val scaleX = width.toFloat() / bitmap.width
        val scaleY = height.toFloat() / bitmap.height
        return scaleX.coerceAtLeast(scaleY)
    }

    // 计算绘制位置
    private fun calculateBitmapPosition(bitmap: Bitmap, scale: Float): Pair<Float, Float> {
        val scaledWidth = bitmap.width * scale
        val scaledHeight = bitmap.height * scale
        return Pair((width - scaledWidth) / 2f, (height - scaledHeight) / 2f)
    }

    // 创建剪切路径
    private fun createClippingPath(): Path {
        return Path().apply {
            fillType = Path.FillType.EVEN_ODD
            addRect(0f, 0f, width.toFloat(), height.toFloat(), Path.Direction.CW)
            addOval(ovalRect, Path.Direction.CW)
        }
    }

    // 设置绘制椭圆内部预览图
    fun setOverlayBitmap(bitmap: Bitmap?) {
        overlayBitmap = bitmap?.let {
            val scale = calculateOvalScale(it, ovalRect.width(), ovalRect.height())
            Bitmap.createScaledBitmap(it, (it.width * scale).toInt(), (it.height * scale).toInt(), true)
        }
        invalidate()
    }

    // 控制是否绘制椭圆内部预览图
    fun setShouldDrawOverlay(preview: CheckPreview = CheckPreview.NONE, promptText: String = "正在进行人脸识别... 请稍等...") {
        if (firstPreview) {
//            if(TempDataStorage.initCamerax){
            mPromptText = promptText
//            }else{
//                promptText = "启动相机失败，检测USB摄像头是否插好，再重新打开界面"
//            }
            firstPreview = false
        }
        currentPreview = preview
        invalidate()
    }

    //更新提示语
    fun refreshPrompt(text: String) {
        this.mPromptText = text
    }

    // 扫描线Bitmap
    private var scanLineBitmap: Bitmap? = null
    private var glowBitmap: Bitmap? = null // 光晕效果
    override fun onSizeChanged(w: Int, h: Int, oldw: Int, oldh: Int) {
        super.onSizeChanged(w, h, oldw, oldh)
//        val ovalWidth = w * 0.7f
//        val ovalHeight = ovalWidth * 1.2f
//        ovalRect.set((w - ovalWidth) / 2, (h - ovalHeight) / 2, (w + ovalWidth) / 2, (h + ovalHeight) / 2)

        // 将50dp转换为像素值
        val topMargin = 200f.dpToPx()

        // 椭圆尺寸计算
        val ovalWidth = w * 0.7f
        val ovalHeight = ovalWidth * 1.2f

        // 设置椭圆位置（顶部从50dp开始）
        // left (保持水平居中)
        // top (直接使用50dp转换值)
        // right (保持水平居中)
        // bottom (根据高度自动计算)
        ovalRect.set((w - ovalWidth) / 2, topMargin, (w + ovalWidth) / 2, topMargin + ovalHeight)
        // 增强渐变效果（从上到下）
        scanGradient =
                LinearGradient(ovalRect.left, ovalRect.top, ovalRect.left, ovalRect.top + scanHeight, intArrayOf(
                    Color.parseColor("#16B9E9"),
                    Color.parseColor("#16B9E9"),
                    Color.parseColor("#16B9E9"),

                    ), null, Shader.TileMode.CLAMP).also {
                    scanPaint.shader = it

                }
        // 计算圆形的半径
        circleRadius = min(ovalWidth, ovalHeight) * 0.2f
        // 生成扫描线Bitmap（核心渐变）
        scanLineBitmap?.recycle()
        scanLineBitmap =
                createScanLineBitmap(width = ovalRect.width().toInt(), height = scanHeight.toInt(), colors = intArrayOf(Color.parseColor("#167DD8F2"), Color.parseColor("#7DD8F2"), Color.parseColor("#167DD8F2")))
        // 生成光晕Bitmap（顶部加强光效）
        glowBitmap?.recycle()
        glowBitmap =
                createScanLineBitmap(width = ovalRect.width().toInt(), height = (scanHeight * 1.8f).toInt(), colors = intArrayOf(Color.parseColor("#087DD8F2"), Color.parseColor("#307DD8F2"), Color.parseColor("#087DD8F2")))
    }

    private fun createScanLineBitmap(width: Int, height: Int, colors: IntArray): Bitmap {
        val bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888)
        val canvas = Canvas(bitmap)
        val paint = Paint().apply {
            shader =
                    LinearGradient(0f, 0f, 0f, height.toFloat(), colors, null, Shader.TileMode.CLAMP)
        }
        canvas.drawRect(0f, 0f, width.toFloat(), height.toFloat(), paint)
        return bitmap
    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)
        // 绘制全屏背景图
        drawClippedBitmapBackground(canvas, backgroundBitmap, backgroundPaint)

        // 绘制局域内背景图
        if (currentPreview != CheckPreview.NONE) {
            drawClippedBitmapPreview(canvas, overlayBitmap, overlayPaint)
        }
        // 1. 绘制高对比度椭圆边框
        canvas.drawOval(ovalRect, borderPaint)

        // 2. 双重区域限制（确保扫描线不溢出） 3. 绘制增强版扫描线 4. 添加扫描线光晕效果
        drawScanLine(canvas)

        // 5. 绘制文字
        drawPromptText(canvas)

        // 6. 绘制打勾打叉状态
        if (currentState != CheckState.NONE) {
            // 在椭圆中心绘制圆形
            drawCircleWithAnimatedMark(canvas)
        }
    }

    override fun onAttachedToWindow() {
        super.onAttachedToWindow()
        scanAnimator.start()
    }

    override fun onDetachedFromWindow() {
        super.onDetachedFromWindow()
        faceScanCompleteListener = null
        scanLineBitmap?.recycle()
        glowBitmap?.recycle()
        // 释放动画
        scanAnimator.cancel()
        markAnimator.cancel()
        // 释放Bitmap资源
        overlayBitmap?.recycle()
        overlayBitmapNull?.recycle()
        backgroundBitmap?.recycle()
        backgroundBitmap?.recycle()

    }
}
