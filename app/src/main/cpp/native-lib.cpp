#include <jni.h>
#include <string>
#include <cstdlib>
#include <android/log.h>
#include <node.h>

#include <libplatform/libplatform.h>
#include <v8.h>

#define APPNAME "MobileAgentCPP"

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
    std::string hello = "Hello from JNI C++ ! Compiled with ABI " ABI "";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jint JNICALL
Java_com_exprograma_mobile_mobileagentcpp_MainActivity_startNodeWithArguments(
        JNIEnv *env,
        jobject /* this */,
        jobjectArray arguments) {

    jsize argument_count = env->GetArrayLength(arguments);

    int c_arguments_size = 0;
    for (int i = 0; i < argument_count ; i++) {
        c_arguments_size += strlen(env->GetStringUTFChars((jstring)env->GetObjectArrayElement(arguments, i), 0));
        c_arguments_size++; // for '\0'
    }

    char* args_buffer = (char*) calloc(c_arguments_size, sizeof(char));

    char* argv[argument_count];

    char* current_args_position = args_buffer;

    for (int i = 0; i < argument_count ; i++)
    {
        const char* current_argument = env->GetStringUTFChars((jstring)env->GetObjectArrayElement(arguments, i), 0);

        strncpy(current_args_position, current_argument, strlen(current_argument));

        argv[i] = current_args_position;

        current_args_position += strlen(current_args_position) + 1;
    }

    int node_result = node::Start(argument_count, argv);
    free(args_buffer);

    return jint(node_result);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_exprograma_mobile_mobileagentcpp_MainActivity_runNativeV8FromSnapshot(JNIEnv *env, jobject instance,
                                                                               jobjectArray arguments) {

    jsize argument_count = env->GetArrayLength(arguments);

    int c_arguments_size = 0;
    for (int i = 0; i < argument_count; i++) {
        c_arguments_size += strlen(env->GetStringUTFChars((jstring) env->GetObjectArrayElement(arguments, i), 0));
        c_arguments_size++; // for '\0'
    }

    char *args_buffer = (char *) calloc(c_arguments_size, sizeof(char));

    char *argv[argument_count];

    char *current_args_position = args_buffer;

    for (int i = 0; i < argument_count; i++) {
        const char *current_argument = env->GetStringUTFChars((jstring) env->GetObjectArrayElement(arguments, i), 0);

        strncpy(current_args_position, current_argument, strlen(current_argument));

        argv[i] = current_args_position;

        current_args_position += strlen(current_args_position) + 1;
    }

//    jclass clazz = env->FindClass("com/exprograma/mobile/mobileagentcpp/JNI");
//    jmethodID getLoc = env->GetStaticMethodID(clazz, "getExternalStorageDir", "()Ljava/lang/String;");
//    jobject result = env->CallStaticObjectMethod(clazz, getLoc);
//    const char *str = env->GetStringUTFChars((jstring) result, NULL);
//    __android_log_print(ANDROID_LOG_WARN, APPNAME, "getExternalStorageDir: %s", str);

    __android_log_print(ANDROID_LOG_WARN, APPNAME, "runNativeV8FromSnapshot: %s\n", argv[0]);

    v8::V8::InitializeICU();
    v8::V8::InitializeExternalStartupData(argv[0]);
    v8::Platform *platform = v8::platform::CreateDefaultPlatform();
    v8::V8::InitializePlatform(platform);
    v8::V8::Initialize();

    // Create a new Isolate and make it the current one.
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::Isolate *isolate = v8::Isolate::New(create_params);
    {
        v8::Isolate::Scope isolate_scope(isolate);
        // Create a stack-allocated handle scope.
        v8::HandleScope handle_scope(isolate);
        // Create a new context.
        v8::Local<v8::Context> context = v8::Context::New(isolate);
        // Enter the context for compiling and running the hello world script.
        v8::Context::Scope context_scope(context);
        // Create a string containing the JavaScript source code.
        v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, "'hello'+'hello'", v8::NewStringType::kNormal).ToLocalChecked();
        // Compile the source code.
        v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();
        // Run the script to get the result.
        v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
        // Convert the result to an UTF8 string and print it.
        v8::String::Utf8Value utf8(result);

        printf("%s\n", *utf8);

        __android_log_print(ANDROID_LOG_WARN, APPNAME, "printf: %s\n", *utf8);

        //Call to js function
        v8::Handle<v8::Object> global = context->Global();
        v8::Handle<v8::Value> function = global->Get(v8::String::NewFromUtf8(isolate, "addStr"));

        if (function->IsFunction()) {
            v8::Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(function);
            int func_argc = 2;
            v8::Handle<v8::Value> func_argv[func_argc];
            func_argv[0] = v8::String::NewFromUtf8(isolate, "Hello");
            func_argv[1] = v8::String::NewFromUtf8(isolate, ", world!");
            v8::Handle<v8::Value> func_result = func->Call(global, func_argc, func_argv);

            v8::String::Utf8Value func_utf8(func_result);

            __android_log_print(ANDROID_LOG_WARN, APPNAME, "printf: %s\n", *func_utf8);
        } else {
            __android_log_print(ANDROID_LOG_WARN, APPNAME, "printf: %s\n", "Not a function");
        }
    }

    // Dispose the isolate and tear down V8.
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();

    return env->NewStringUTF("Hello from JNI C++");
}
