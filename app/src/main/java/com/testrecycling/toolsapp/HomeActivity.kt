package com.testrecycling.toolsapp
import android.os.Bundle
import androidx.activity.viewModels
import androidx.camera.lifecycle.ProcessCameraProvider
import com.testrecycling.toolsapp.utils.Loge
import com.testrecycling.toolsapp.databinding.ActivityHomeBinding
import com.testrecycling.toolsapp.fitsystembar.base.bind.BaseBindActivity
import com.testrecycling.toolsapp.ui.MainFragment
import com.testrecycling.toolsapp.ui.TestFaceFragment
import com.testrecycling.toolsapp.utils.AppUtils
import com.testrecycling.toolsapp.vm.CabinetVM
import dagger.hilt.android.AndroidEntryPoint
import kotlinx.coroutines.launch


@AndroidEntryPoint
class HomeActivity : BaseBindActivity<ActivityHomeBinding>() {
    private val cabinetVM: CabinetVM by viewModels()
    override fun layoutRes(): Int {
        return R.layout.activity_home
    }


    override fun initialize(savedInstanceState: Bundle?) {
        cabinetVM.ioScope.launch {
            // 预热相机Provider 快速启动相机能从6秒到2秒
            cabinetVM.cameraProviderFuture = ProcessCameraProvider.getInstance(this@HomeActivity)
        }
        navigateToHome()
    }


    override fun onDetachedFromWindow() {
        Loge.e("调试socket home onDestroy")
        super.onDetachedFromWindow()
    }
    private fun navigateToHome() {
        binding.tvVersion.text = "版本号：v${AppUtils.getVersionName()}"
        Loge.e("调试socket navigateToHome 根fragment ${fragmentCoordinator.getRootFragment()}")
        if (fragmentCoordinator.getRootFragment() == null) {
            Loge.e("调试socket navigateToHome 根fragment 加载单格口")
            fragmentCoordinator.setRootFragment(fragmentClass = MainFragment::class.java, args = Bundle().apply {
            })
        }
    }
    override fun onDestroy() {
        Loge.e("调试socket home onDestroy")
        super.onDestroy()
    }
}