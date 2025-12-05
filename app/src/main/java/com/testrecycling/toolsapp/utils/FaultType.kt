package com.testrecycling.toolsapp.utils

/***
 * 故障状态
 */
object FaultType {
    /***
     * 1.投送门开门异常
     * 2.投递门关门异常
     * 3.清运门开门异常
     * 4.清运门关门异常
     * 5.摄像头异常
     * 6.电磁锁异常
     * 7:内灯异常
     * 8:外灯异常
     * 9:推杆异常
     * @see 1.投送门开门异常 111 121
     * @see 2.投递门关门异常 110 120
     * @see 3.清运门开门异常 311 321 331
     * @see 4.清运门关门异常 410 420 430
     * @see 5.摄像头异常 51 52
     * @see 6.电磁锁异常
     * @see 7:内灯异常
     * @see 8:外灯异常
     * @see 9:推杆异常 91 92
     * @see 11:1红外满溢
     * @see 12:2红外满溢
     * @see 211:超重满溢
     * @see 212:超重满溢
     * @see 5101:格口一校准状态
     * @see 5102:格口一故障状态
     * @see 5201:格口二校准状态
     * @see 5202:格口二故障状态
     */

    const val FAULT_CODE_0 = 0
    const val FAULT_CODE_111 = 111
    const val FAULT_CODE_121 = 121
    const val FAULT_CODE_110 = 110
    const val FAULT_CODE_120 = 120
    const val FAULT_CODE_311 = 311
    const val FAULT_CODE_321 = 321
    const val FAULT_CODE_331 = 331
    const val FAULT_CODE_410 = 410
    const val FAULT_CODE_420 = 420
    const val FAULT_CODE_430 = 430
    const val FAULT_CODE_51 = 51
    const val FAULT_CODE_52 = 52
    const val FAULT_CODE_5 = 5
    const val FAULT_CODE_6 = 6
    const val FAULT_CODE_7 = 7
    const val FAULT_CODE_8 = 8
    const val FAULT_CODE_91 = 91
    const val FAULT_CODE_92 = 92
    const val FAULT_CODE_11 = 11
    const val FAULT_CODE_12 = 12
    const val FAULT_CODE_211 = 211
    const val FAULT_CODE_212 = 212
    const val FAULT_CODE_5101 = 5101
    const val FAULT_CODE_5102 = 5102
    const val FAULT_CODE_5201 = 5201
    const val FAULT_CODE_5202 = 5202


}