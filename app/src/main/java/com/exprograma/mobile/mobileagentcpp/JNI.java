package com.exprograma.mobile.mobileagentcpp;

import android.util.Log;

public class JNI {
    public static int test(int n) {
        Log.w("MobileAgentCPP-Java","test");
        return n*10;
    }
}
