package com.testrecycling.toolsapp.utils

/**
 * socket 协议标签
 */
object CmdValue {
    /***
     *正在连接
     * connecting
     */
    const val CONNECTING = "connecting"

    /***
     *接收
     * receive
     */
    const val RECEIVE = "receive"

    /***
     *发送
     * send
     */
    const val SEND = "send"

    /***
     *心跳
     * heartBeat
     */
    const val CMD_HEART_BEAT = "heartBeat"

    /***
     *登录
     * login
     */
    const val CMD_LOGIN = "login"

    /***
     *初始化排至
     * initConfig
     */
    const val CMD_INIT_CONFIG = "initConfig"

    /***
     *打开格口
     * openDoor
     */
    const val CMD_OPEN_DOOR = "openDoor"

    /***
     *关闭格口
     * closeDoor
     */
    const val CMD_CLOSE_DOOR = "closeDoor"

    /***
     *手机号登录选择格口
     * phoneNumberLogin
     */
    const val CMD_PHONE_NUMBER_LOGIN = "phoneNumberLogin"

    /***
     *手机号开格口
     * phoneUserOpenDoor
     */
    const val CMD_PHONE_USER_OPEN_DOOR = "phoneUserOpenDoor"

    /***
     *重启
     * restart
     */
    const val CMD_RESTART = "restart"

    /***
     *ota更新
     * ota
     */
    const val CMD_OTA = "ota"

    /***
     *ota/apk更新
     * ota/apk
     */
    const val CMD_OTA_APK = "ota/apk"

    /***
     *debug更新
     * debug
     */
    const val CMD_DEBUG = "debug"

    /***
     *fault更新
     * fault
     */
    const val CMD_FAULT = "fault"

    /***
     *上传日志
     * uploadLog
     */
    const val CMD_UPLOAD_LOG = "uploadLog"

    /***
     * 控制设备拍照上传响应
     * admin/photo
     */
    const val CMD_ADMIN_PHOTO = "admin/photo"

    /***
     * 控制设备去皮响应
     * admin/peel
     */
    const val CMD_ADMIN_PEEL = "admin/peel"

    /***
     * 控制设备开门响应
     * admin/opsDoor
     */
    const val CMD_ADMIN_OPSDOOR = "admin/opsDoor"
}