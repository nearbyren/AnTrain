package com.testrecycling.toolsapp

import android.content.Intent
import android.os.Bundle
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch

/***
 * 这里考虑优化启动逻辑处理问题
 */
class StartUiActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_start_ui)
        CoroutineScope(Dispatchers.Main).launch {
            delay(50)
            startActivity(Intent(this@StartUiActivity, HomeActivity::class.java))
        }
    }
}