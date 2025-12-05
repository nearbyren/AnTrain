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
import com.testrecycling.toolsapp.databinding.FragmentMainBinding
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
class MainFragment : BaseBindFragment<FragmentMainBinding>() {
    override fun isShowActionBar(): Boolean {
        return true
    }

    override fun isShowActionBarBack(): Boolean {
        return true
    }

    override fun doneCountdown() {
    }

    override fun layoutRes(): Int {
        return R.layout.fragment_main
    }

    override fun initialize(savedInstanceState: Bundle?) {

        binding.testFace.setOnClickListener {
            mActivity?.navigateTo(fragmentClass = TestFaceFragment::class.java)
        }
    }
}