package com.testrecycling.toolsapp.utils

import com.testrecycling.toolsapp.utils.Loge
import java.math.BigDecimal
import java.math.RoundingMode

/**
 * @author: lr
 * @created on: 2025/11/2 下午5:12
 * @description:
 */
object CalculationUtil {
    /***
     * 相减
     * @param after
     * @param before
     */
    fun subtractFloats(after: String, before: String): String {
        val bd1 = BigDecimal(after)
        val bd2 = BigDecimal(before)
        return bd1.subtract(bd2).setScale(2, RoundingMode.HALF_UP).toString()
    }

    /***
     * 相乘
     * @param after
     * @param before
     */
    fun multiplyFloats(after: String, before: String): String {
        val bd1 = BigDecimal(after)
        val bd2 = BigDecimal(before)
        return bd1.multiply(bd2).setScale(2, RoundingMode.HALF_UP).toString()
    }

    /***
     * 计算获取百分比
     * @param weight
     * @param totalWeight
     */
    fun getWeightPercent(weight: Float, totalWeight: Float): String {
        return if (totalWeight == 0.00f) {
            "0%"// 避免除零错误
        } else {
            val result = (weight / totalWeight) * 100
            "$result%"
        }
    }
    /**
     * 获取重量
     * @param weight
     */
    fun getWeight(weight: Int): String {
        Loge.e("换算重量前 $weight")
        if (weight <= 0) return "0.00"
        return "%.2f".format(weight.toDouble() / 1000)
//        return new BigDecimal(weight).divide(BigDecimal.valueOf(1000)).setScale(2, RoundingMode.HALF_UP).toString();

    }
}