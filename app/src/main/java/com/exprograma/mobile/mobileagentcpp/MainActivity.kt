package com.exprograma.mobile.mobileagentcpp

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        sample_text.text = stringFromJNI().toString()
    }

    fun runV8(view:View) {
        sample_text.text = stringFromJNINew("node","hello").toString()
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    external fun stringFromJNINew(vararg argv:String): String

    external fun stringFromCPP(): Int

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
//            System.loadLibrary("node");
            System.loadLibrary("native-lib")
            System.loadLibrary("native-math")
            System.loadLibrary("native-activity")
        }
    }
}
