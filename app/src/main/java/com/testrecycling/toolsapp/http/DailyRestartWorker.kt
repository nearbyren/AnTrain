package com.testrecycling.toolsapp.http

/**
 * @author: lr
 * @created on: 2025/5/14 上午11:25
 * @description:指定时间重启
 */
import android.content.Context
import android.content.Intent
import androidx.work.CoroutineWorker
import androidx.work.WorkerParameters
import com.testrecycling.toolsapp.utils.Loge
import com.testrecycling.toolsapp.FaceApplication

/***
 * 指定时间重启
 */
class DailyRestartWorker(
    context: Context, params: WorkerParameters,
) : CoroutineWorker(context, params) {

    override suspend fun doWork(): Result {
        restart()
        return Result.success()
    }

    private fun restart() {
        try {
            FaceApplication.getInstance().baseActivity?.let { act ->
                val packageManager = act.packageManager
                val intent = packageManager.getLaunchIntentForPackage(act.packageName)
                val componentName = intent?.component
                val mainIntent = Intent.makeRestartActivityTask(componentName)
                act.finishAffinity() // 关闭所有Activity
                act.startActivity(mainIntent)
                Runtime.getRuntime().exit(0) // 确保完全
            }
            Loge.d("指定时间重启动app... ")
        } catch (e: Exception) {
            Loge.d("指定时间重启动app... ${e.message}")
        }
    }

}