package com.testrecycling.toolsapp.utils;


import android.content.pm.PackageManager;
import android.telephony.SubscriptionInfo;
import android.telephony.SubscriptionManager;
import android.util.Log;

import androidx.core.app.ActivityCompat;

import com.testrecycling.toolsapp.utils.AppUtils;

import java.util.List;

public class IccidOper {
    //在自己内部定义自己的一个实例，只供内部调用
    private static final IccidOper instance = new IccidOper();

    //这里提供了一个供外部访问本class的静态方法，可以直接访问
    public static IccidOper getInstance() {
        return instance;
    }
    private static String TAG = "IccidOper";
    public String GetIccid() {
        SubscriptionManager sm = null;
        String iccid = null;
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP_MR1) {
            sm = SubscriptionManager.from(AppUtils.getContext());
            if (ActivityCompat.checkSelfPermission(AppUtils.getContext(), android.Manifest.permission.READ_PHONE_STATE) != PackageManager.PERMISSION_GRANTED) {
                return null;
            }
            int count = sm.getActiveSubscriptionInfoCount();
            int max = sm.getActiveSubscriptionInfoCountMax();
            Log.d(TAG, "card number=" + String.valueOf(count));
            Log.d(TAG, "card slot num" + String.valueOf(max));
            List<SubscriptionInfo> sis = null;
            sis = sm.getActiveSubscriptionInfoList();
            for (SubscriptionInfo subInfo : sis) {
                iccid = subInfo.getIccId();
                Log.d(TAG, "iccid=" + iccid);
            }
        }
        return iccid;
    }
}
