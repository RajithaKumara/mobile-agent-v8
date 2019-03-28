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

        sample_text.text = stringFromJNI()
        fileSystem = FileSystem()
    }

    fun eval(view: View) {
        var nodeThread = Thread(Runnable {
            startNodeWithArguments("node", "-e", fileContent!!)
        })
        nodeThread?.start()
        printLog("LOG: (eval) " + nodeThread?.name + " up")
    }

    fun loadContent(view: View) {
        try {
            fileContent = fileSystem?.getFileContent(filePath.text.toString())
            printLog(fileContent)
        } catch (e: Throwable) {
            printLog("Error (loadContent): " + e.message)
        }

    }

    fun printLog(text: String?) {
        var temp = sample_text.text.toString()
        temp += "\n\n" + text
        sample_text.text = temp
    }

    external fun stringFromJNI(): String

    external fun startNodeWithArguments(vararg argv: String): String

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }
}
