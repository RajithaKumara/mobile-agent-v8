package com.exprograma.mobile.mobileagentcpp;

import android.os.Environment;
import android.util.Log;

public class JNI {
    public void test(String[] args) {
        Log.w("MobileAgentCPP-Java",args[1]);
    }

    public static String getExternalStorageDir() {
        return Environment.getExternalStorageDirectory().getAbsolutePath() + "/FYP/v8data/";
    }
}
