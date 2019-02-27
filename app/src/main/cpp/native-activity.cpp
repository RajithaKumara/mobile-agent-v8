//
// Created by rajitha on 2/6/19.
//
#include <jni.h>
//#include <string>
//#include <sstream>

#include <libplatform/libplatform.h>
#include <v8.h>

//#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include <node.h>

//#include <agent_math.h>
//
//extern "C" JNIEXPORT jstring JNICALL
//Java_com_exprograma_mobile_mobileagentcpp_MainActivity_stringFromJNINew(JNIEnv *env, jobject /* this */) {
//    std::string hello = "Hello from JNI through agent_math C++";
//    agent_math agent_integer(hello);
//    std::string result;
//    result = agent_integer.getNum();
////    std::stringstream ss;
////    ss << result;
////    + ss.str()
//
//    return env->NewStringUTF(result.c_str());
//}

//extern "C" JNIEXPORT jstring JNICALL
//Java_com_exprograma_mobile_mobileagentcpp_MainActivity_stringFromJNINew(JNIEnv *env, jobject /* this */) {
//
//    std::string hello = "Hello from JNI activity C++";
//    return env->NewStringUTF(hello.c_str());
//}

extern "C" JNIEXPORT jstring JNICALL
Java_com_exprograma_mobile_mobileagentcpp_MainActivity_stringFromJNINew(JNIEnv *env, jobject instance, jobjectArray args) {
//     Prepare fake argv commandline arguments
//    auto continuousArray = app::makeContinuousArray(env, args);
//    auto argv = app::getArgv(continuousArray);
//
//    v8::V8::InitializeICUDefaultLocation(argv[0]);
//    v8::V8::InitializeExternalStartupData(argv[0]);

//    v8::V8::InitializeICU();
    v8::Platform *platform = v8::platform::CreateDefaultPlatform();
    v8::V8::InitializePlatform(platform);

//    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
//    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();
//
//    // Create a new Isolate and make it the current one.
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::Isolate* isolate = v8::Isolate::New(create_params);
    {
        v8::Isolate::Scope isolate_scope(isolate);
        // Create a stack-allocated handle scope.
        v8::HandleScope handle_scope(isolate);
        // Create a new context.
        v8::Local<v8::Context> context = v8::Context::New(isolate);
        // Enter the context for compiling and running the hello world script.
        v8::Context::Scope context_scope(context);
        // Create a string containing the JavaScript source code.
        v8::Local<v8::String> source =
                v8::String::NewFromUtf8(isolate, "'Hello' + ', World!'",
                                        v8::NewStringType::kNormal)
                        .ToLocalChecked();
        // Compile the source code.
        v8::Local<v8::Script> script =
                v8::Script::Compile(context, source).ToLocalChecked();
        // Run the script to get the result.
        v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
        // Convert the result to an UTF8 string and print it.
        v8::String::Utf8Value utf8(isolate, result);
        printf("%s\n", *utf8);
    }
    // Dispose the isolate and tear down V8.
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete create_params.array_buffer_allocator;

    std::string hello = v8::V8::GetVersion();

//    std::string hello = "Hello from JNI activity C++";
    return env->NewStringUTF(hello.c_str());
}



//namespace app {
//std::vector<char> makeContinuousArray(JNIEnv *env, jobjectArray fromArgs)
//{
//    int count = env->GetArrayLength(fromArgs);
//    std::vector<char> buffer;
//    for (int i = 0; i < count; i++)
//    {
//        jstring str = (jstring)env->GetObjectArrayElement(fromArgs, i);
//        const char* sptr = env->GetStringUTFChars(str, 0);
//
//        do {
//            buffer.push_back(*sptr);
//        }
//        while(*sptr++ != '\0');
//    }
//
//    return buffer;
//}
//
//std::vector<char*> getArgv(std::vector<char>& fromContinuousArray)
//{
//    std::vector<char*> argv;
//
//    argv.push_back(fromContinuousArray.data());
//    for (int i = 0; i < fromContinuousArray.size() - 1; i++)
//        if (fromContinuousArray[i] == '\0') argv.push_back(&fromContinuousArray[i+1]);
//
//    argv.push_back(nullptr);
//
//    return argv;
//}
//}