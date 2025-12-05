package com.testrecycling.toolsapp.model

import androidx.room.ColumnInfo
import androidx.room.ColumnInfo.Companion.INTEGER
import androidx.room.ColumnInfo.Companion.TEXT
import androidx.room.Entity
import androidx.room.Ignore
import androidx.room.PrimaryKey

/***
 * 用户表
 */
@Entity(tableName = "UserInfo")
class UserEntity(
    /***
     * 用户主键id
     */
    @PrimaryKey(autoGenerate = true) @ColumnInfo(name = "id", typeAffinity = INTEGER) var id: Long = 0,

    /***
     * 用户id
     */
    @ColumnInfo(name = "userId", typeAffinity = TEXT, defaultValue = "") var userId: String? = null,

    /***
     * 网络用户ID
     */
    @ColumnInfo(name = "tableId", typeAffinity = TEXT, defaultValue = "") var tableId: String? = null,
    /***
     *  省
     */
    @ColumnInfo(name = "tableProvince", typeAffinity = TEXT, defaultValue = "") var tableProvince: String? = null,
    /***
     * 市
     */
    @ColumnInfo(name = "tableCity", typeAffinity = TEXT, defaultValue = "") var tableCity: String? = null,
    /***
     *  县
     */
    @ColumnInfo(name = "tableCounty", typeAffinity = TEXT, defaultValue = "") var tableCounty: String? = null,
    /***
     * 单位
     */
    @ColumnInfo(name = "tableUnit", typeAffinity = TEXT, defaultValue = "") var tableUnit: String? = null,
    /***
     * 用户工号
     */
    @ColumnInfo(name = "userNo", typeAffinity = TEXT, defaultValue = "") var userNo: String? = null,
    /***
     * 用户名称
     */
    @ColumnInfo(name = "userName", typeAffinity = TEXT, defaultValue = "") var userName: String? = null,
    /***
     * 用户账号
     */
    @ColumnInfo(name = "userAccount", typeAffinity = TEXT, defaultValue = "") var userAccount: String? = null,
    /***
     * 用户密码
     */
    @ColumnInfo(name = "userPassword", typeAffinity = TEXT, defaultValue = "") var userPassword: String? = null,
    /***
     *网络头像
     */
    @ColumnInfo(name = "netAvatar", typeAffinity = TEXT, defaultValue = "") var netAvatar: String? = null,
    /***
     * 本地头像
     */
    @ColumnInfo(name = "userAvatar", typeAffinity = TEXT, defaultValue = "") var userAvatar: String? = null,
    /***
     * 时间
     */
    @ColumnInfo(name = "time", typeAffinity = TEXT, defaultValue = "") var time: String? = null,
    /***
     * -1.注销 0.禁用1.可用
     */
    @ColumnInfo(name = "userStatus", typeAffinity = INTEGER, defaultValue = 0.toString()) var userStatus: Int = 1,
    /***
     * 用户人脸特征
     */
    @ColumnInfo(name = "featureData", typeAffinity = TEXT) var featureData: FloatArray? = null,
) {
    @Ignore
    constructor() : this(0, null, null, null, null, null, null, null, null, null, null, null, null, null, 1, null)

    override fun toString(): String {
        return "id=$id,userId=${userId},tableId=${tableId},tableProvince=${tableProvince},tableCity=${tableCity},tableCounty=${tableCounty},tableUnit=${tableUnit},userNo=${userNo},userName=${userName},userAccount=${userAccount},userPassword=${userPassword},netAvatar=${netAvatar},userAvatar=${userAvatar},time=${time},userStatus=${userStatus}"
    }
}

