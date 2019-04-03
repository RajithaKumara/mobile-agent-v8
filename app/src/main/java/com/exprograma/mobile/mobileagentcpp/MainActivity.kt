package com.exprograma.mobile.mobileagentcpp

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import kotlinx.android.synthetic.main.activity_main.*
import java.lang.Exception
import java.lang.IllegalArgumentException

class MainActivity : AppCompatActivity() {

    var fileContent: String? = null
    var fileSystem: FileSystem? = null
    lateinit var v8Thread: Thread

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        sample_text.text = stringFromJNI()
        fileSystem = FileSystem()
        printLog(fileSystem?.copyAssetsToFilesDir(assets, filesDir))
    }

    fun runV8(view: View) {
        try {
            v8Thread = Thread(Runnable {
                if (fileContent == null) {
                    fileContent = ""
                }
                runNativeV8(filesDir.absolutePath + "/", fileContent!!)
            })
//            v8Thread.priority = Thread.MAX_PRIORITY
            v8Thread.start()
            printLog("LOG (runV8): " + v8Thread.name + " up")
//        } catch (e:IllegalArgumentException) {
//            printLog("Error (runV8): " + e.message)
        } catch (e: Exception) {
            printLog("Error (runV8): " + e.message)
        }
    }

    fun loadContent(view: View) {
        try {
            fileContent = fileSystem?.getFileContent(filePath.text.toString())
            printLog(fileContent)
        } catch (e: Throwable) {
            printLog("Error (loadContent): " + e.message)
        }

    }

    fun checkThread(view: View) {
        printLog("Log (checkThread): " + v8Thread.isAlive)
    }

    fun callFunc(view: View) {
//        var v8Thread = Thread(Runnable {
        if (fileContent == null) {
            fileContent = ""
        }
        callV8Func(fileContent!!)
//        })
//        v8Thread.start()
//        printLog("LOG (runV8): " + v8Thread.name + " up")
    }

    fun runV8FromSnapshot(view: View) {
//        var v8Thread = Thread(Runnable {
        if (fileContent == null) {
            fileContent = ""
        }
        runNativeV8FromSnapshot(filesDir.absolutePath, fileContent!!)
//        })
//        v8Thread.start()
//        printLog("LOG (runV8): " + v8Thread.name + " up")
    }

    fun printLog(text: String?) {
        var temp = sample_text.text.toString()
        temp += "\n\n" + text
        sample_text.text = temp
    }

    external fun stringFromJNI(): String

    external fun runNativeV8(vararg argv: String): String

    external fun callV8Func(vararg argv: String): String

    external fun runNativeV8FromSnapshot(vararg argv: String): String

    external fun stringFromCPP(): Int

    companion object {
        init {
            System.loadLibrary("native-lib")
            System.loadLibrary("native-activity")
        }
    }
}
