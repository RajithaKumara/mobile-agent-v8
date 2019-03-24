#include <jni.h>
#include <string>

#include <v8.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_exprograma_mobile_mobileagentcpp_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {

#if defined(__arm__)
    #if defined(__ARM_ARCH_7A__)
    #if defined(__ARM_NEON__)
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a/NEON (hard-float)"
      #else
        #define ABI "armeabi-v7a/NEON"
      #endif
    #else
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a (hard-float)"
      #else
        #define ABI "armeabi-v7a"
      #endif
    #endif
  #else
   #define ABI "armeabi"
  #endif
#elif defined(__i386__)
#define ABI "x86"
#elif defined(__x86_64__)
#define ABI "x86_64"
#elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
#define ABI "mips64"
#elif defined(__mips__)
#define ABI "mips"
#elif defined(__aarch64__)
#define ABI "arm64-v8a"
#else
#define ABI "unknown"
#endif

    std::string v8Ver = v8::V8::GetVersion();
    std::string hello = "Hello from JNI C++ ! Compiled with ABI " ABI ". V8 version: "+v8Ver;
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jint JNICALL
Java_com_exprograma_mobile_mobileagentcpp_MainActivity_stringFromCPP(JNIEnv *env, jobject /* this */) {
    return env->GetVersion();
}
