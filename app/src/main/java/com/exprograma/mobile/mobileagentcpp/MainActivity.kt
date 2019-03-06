package com.exprograma.mobile.mobileagentcpp

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    var fileContent: String? = null
    var fileSystem: FileSystem? = null
    lateinit var v8Thread: Thread

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        sample_text.text = stringFromJNI().toString()
        fileSystem = FileSystem()
    }

    fun runV8(view:View) {
        v8Thread = Thread(Runnable {
            if (fileContent == null) {
                fileContent = ""
            }
            stringFromJNINew(fileContent!!)
        })
        v8Thread.priority = Thread.MAX_PRIORITY
        v8Thread.start()
        printLog("LOG (runV8): " + v8Thread.name + " up")
    }

    fun loadContent(view: View) {
        try {
            fileContent = fileSystem?.getFileContent(filePath.text.toString())
            printLog(fileContent)
        } catch (e: Throwable) {
            printLog("Error (loadContent): " + e.message)
        }

    }

    fun checkThread(view: View){
        printLog("Log (checkThread): " + v8Thread.isAlive)
    }

    fun callFunc(view: View){
        v8Thread = Thread(Runnable {
            if (fileContent == null) {
                fileContent = ""
            }
            stringFromJNINew(fileContent!!)
        })
        v8Thread.start()
        printLog("LOG (runV8): " + v8Thread.name + " up")
    }

    fun printLog(text: String?) {
        var temp = sample_text.text.toString()
        temp += "\n\n" + text
        sample_text.text = temp
    }

    external fun stringFromJNI(): String

    external fun stringFromJNINew(vararg argv:String): String

    external fun stringFromCPP(): Int

    companion object {
        init {
            System.loadLibrary("native-lib")
            System.loadLibrary("native-math")
            System.loadLibrary("native-activity")
        }
    }
}
