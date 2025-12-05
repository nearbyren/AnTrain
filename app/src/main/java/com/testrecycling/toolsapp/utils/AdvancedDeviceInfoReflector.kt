package com.testrecycling.toolsapp.utils

import android.content.Context
import android.telephony.TelephonyManager
import java.lang.reflect.Method


class FixedDeviceInfoReflector(private val context: Context) {

    private val telephonyManager: TelephonyManager by lazy {
        context.getSystemService(Context.TELEPHONY_SERVICE) as TelephonyManager
    }

    /**
     * 安全地通过反射调用方法 - 修复版本
     */
    private fun safeReflectInvoke(methodName: String, parameterTypes: Array<Class<*>>? = null, args: Array<Any>? = null): Any? {
        return try {
            val method: Method = if (parameterTypes != null) {
                telephonyManager.javaClass.getMethod(methodName, *parameterTypes)
            } else {
                telephonyManager.javaClass.getMethod(methodName)
            }

            if (args != null) {
                method.invoke(telephonyManager, *args)
            } else {
                method.invoke(telephonyManager)
            }
        } catch (e: Exception) {
            logReflectionError(methodName, e)
            null
        }
    }

    /**
     * 获取设备信息（修复版本）
     */
    fun getDeviceInfo(): DeviceInfo {
        return DeviceInfo(
            imei = getImeiWithMultipleMethods(),
            imsi = getImsiWithMultipleMethods(),
            iccid = getIccidWithMultipleMethods(),
            phoneNumber = getPhoneNumberByReflection(),
            deviceId = getDeviceIdByReflection(),
            simOperator = getSimOperatorByReflection()
        )
    }

    /**
     * 多种方法获取 IMEI - 修复版本
     */
    private fun getImeiWithMultipleMethods(): String? {
        // 方法1: getImei(int)
        for (slot in 0 until 2) {
            // 修复：使用明确的类型转换
            val paramTypes = arrayOf<Class<*>>(java.lang.Integer.TYPE)
            val imei = safeReflectInvoke("getImei", paramTypes, arrayOf(slot)) as? String
            if (!imei.isNullOrEmpty()) return imei
        }

        // 方法2: getDeviceId(int)
        for (slot in 0 until 2) {
            val paramTypes = arrayOf<Class<*>>(java.lang.Integer.TYPE)
            val deviceId = safeReflectInvoke("getDeviceId", paramTypes, arrayOf(slot)) as? String
            if (!deviceId.isNullOrEmpty()) return deviceId
        }

        // 方法3: getDeviceId()
        return safeReflectInvoke("getDeviceId") as? String
    }

    /**
     * 多种方法获取 IMSI - 修复版本
     */
    private fun getImsiWithMultipleMethods(): String? {
        // 方法1: getSubscriberId(int)
        for (slot in 0 until 2) {
            val paramTypes = arrayOf<Class<*>>(java.lang.Integer.TYPE)
            val imsi = safeReflectInvoke("getSubscriberId", paramTypes, arrayOf(slot)) as? String
            if (!imsi.isNullOrEmpty()) return imsi
        }

        // 方法2: getSubscriberId()
        return safeReflectInvoke("getSubscriberId") as? String
    }

    /**
     * 多种方法获取 ICCID - 修复版本
     */
    private fun getIccidWithMultipleMethods(): String? {
        // 方法1: getSimSerialNumber(int)
        for (slot in 0 until 2) {
            val paramTypes = arrayOf<Class<*>>(java.lang.Integer.TYPE)
            val iccid = safeReflectInvoke("getSimSerialNumber", paramTypes, arrayOf(slot)) as? String
            if (!iccid.isNullOrEmpty()) return iccid
        }

        // 方法2: getSimSerialNumber()
        return safeReflectInvoke("getSimSerialNumber") as? String
    }

    /**
     * 获取手机号码
     */
    private fun getPhoneNumberByReflection(): String? {
        return safeReflectInvoke("getLine1Number") as? String
    }

    /**
     * 获取设备ID
     */
    private fun getDeviceIdByReflection(): String? {
        return safeReflectInvoke("getDeviceId") as? String
    }

    /**
     * 获取SIM运营商
     */
    private fun getSimOperatorByReflection(): String? {
        return safeReflectInvoke("getSimOperator") as? String
    }

    /**
     * 获取SIM卡状态 - 修复版本
     */
    fun getSimStateByReflection(slotIndex: Int = 0): Int {
        return try {
            val paramTypes = arrayOf<Class<*>>(java.lang.Integer.TYPE)
            val method: Method = telephonyManager.javaClass.getMethod("getSimState", *paramTypes)
            (method.invoke(telephonyManager, slotIndex) as? Int) ?: -1
        } catch (e: Exception) {
            -1
        }
    }

    /**
     * 检查是否有SIM卡
     */
    fun hasSimCard(slotIndex: Int = 0): Boolean {
        val state = getSimStateByReflection(slotIndex)
        return state != TelephonyManager.SIM_STATE_ABSENT && state != -1
    }

    /**
     * 记录反射错误
     */
    private fun logReflectionError(methodName: String, exception: Exception) {
        println("反射调用 $methodName 失败: ${exception.message}")
    }

    data class DeviceInfo(
        val imei: String?,
        val imsi: String?,
        val iccid: String?,
        val phoneNumber: String?,
        val deviceId: String?,
        val simOperator: String?
    ) {
        fun toMap(): Map<String, String?> {
            return mapOf(
                "IMEI" to imei,
                "IMSI" to imsi,
                "ICCID" to iccid,
                "PhoneNumber" to phoneNumber,
                "DeviceId" to deviceId,
                "SimOperator" to simOperator
            )
        }

        fun isValid(): Boolean {
            return !imei.isNullOrEmpty() || !imsi.isNullOrEmpty() || !iccid.isNullOrEmpty()
        }
    }
}