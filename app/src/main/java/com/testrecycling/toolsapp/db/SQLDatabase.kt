package com.testrecycling.toolsapp.db

import androidx.room.Database
import androidx.room.RoomDatabase
import com.testrecycling.toolsapp.dao.ConfigFlowDao
import com.testrecycling.toolsapp.dao.FileFlowDao
import com.testrecycling.toolsapp.dao.LatticeFlowDao
import com.testrecycling.toolsapp.dao.LogFlowDao
import com.testrecycling.toolsapp.dao.ResFlowDao
import com.testrecycling.toolsapp.dao.StateFlowDao
import com.testrecycling.toolsapp.dao.TransFlowDao
import com.testrecycling.toolsapp.dao.WeightFlowDao
import com.testrecycling.toolsapp.model.ConfigEntity
import com.testrecycling.toolsapp.model.FileEntity
import com.testrecycling.toolsapp.model.LatticeEntity
import com.testrecycling.toolsapp.model.LogEntity
import com.testrecycling.toolsapp.model.ResEntity
import com.testrecycling.toolsapp.model.StateEntity
import com.testrecycling.toolsapp.model.TransEntity
import com.testrecycling.toolsapp.model.WeightEntity

@Database(entities = [
    LatticeEntity::class,
    ConfigEntity::class,
    StateEntity::class,
    TransEntity::class,
    WeightEntity::class,
    ResEntity::class,
    FileEntity::class,
    LogEntity::class], version = 5, exportSchema = false)
abstract class SQLDatabase : RoomDatabase() {

    ///日志操作
    abstract fun logFlow(): LogFlowDao

    ///箱体配置
    abstract fun latticeFlow(): LatticeFlowDao

    ///箱体配置
    abstract fun stateFlow(): StateFlowDao

    ///初始化配置
    abstract fun initConfigFlow(): ConfigFlowDao

    ///打开舱门
    abstract fun transFlowFlow(): TransFlowDao

    ///上报关闭
    abstract fun weightFlowDao(): WeightFlowDao

    ///资源
    abstract fun resFlowDao(): ResFlowDao

    ///文件
    abstract fun fileFlowDao(): FileFlowDao
}
