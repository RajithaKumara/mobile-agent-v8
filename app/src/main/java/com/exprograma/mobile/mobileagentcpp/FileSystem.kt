package com.exprograma.mobile.mobileagentcpp

import android.content.res.AssetManager
import android.os.Environment
import java.io.*

class FileSystem {

    fun isExternalStorageReadable(): Boolean {
        return Environment.getExternalStorageState() in
                setOf(Environment.MEDIA_MOUNTED, Environment.MEDIA_MOUNTED_READ_ONLY)
    }

    fun isExternalStorageWritable(): Boolean {
        return Environment.getExternalStorageState() == Environment.MEDIA_MOUNTED
    }

    fun getExternalStorageDir(): String {
        return Environment.getExternalStorageDirectory().absolutePath
    }

    fun copyAssetsToFilesDir(assetsManager: AssetManager, filesDir: File): String {
        var files = assetsManager.list(V8)
        var str = ""
        for (filePath in files) {
            val file = File(filesDir, filePath)
            copyAsset(assetsManager, V8 + "/" + filePath, file)
            str += filePath + " | "
        }
        return str
    }

    private fun copyAsset(assetManager: AssetManager, fromAssetPath: String, toFile: File): Boolean {
        var inputStream: InputStream? = null
        var outStream: OutputStream? = null
        try {
            inputStream = assetManager.open(fromAssetPath)
            toFile.createNewFile()
            outStream = FileOutputStream(toFile)
            copyFile(inputStream!!, outStream)
            inputStream!!.close()
            inputStream = null
            outStream!!.flush()
            outStream!!.close()
            outStream = null
            return true
        } catch (e: Exception) {
            e.printStackTrace()
            return false
        }
    }

    @Throws(IOException::class)
    private fun copyFile(inputStream: InputStream, outputStream: OutputStream) {
        val buffer = ByteArray(1024)
        var read: Int = -1
        while ({ read = inputStream.read(buffer); read }() != -1) {
            outputStream.write(buffer, 0, read)
        }
    }

    fun getFileContent(fileName: String): String {
        var stream = FileInputStream(getExternalStorageDir() + "/" + fileName)
        val inputReader = InputStreamReader(stream)
        val buffReader = BufferedReader(inputReader)
        var line: String? = null
        var fileContent: String = ""
        while ({ line = buffReader.readLine(); line }() != null) {
            fileContent += line
        }

        return fileContent
    }

    companion object {
        const val V8 = "v8"
    }
}
