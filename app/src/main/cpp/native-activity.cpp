//
// Created by rajitha on 2/6/19.
//
#include <jni.h>

#include <libplatform/libplatform.h>
#include <v8.h>

#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <android/log.h>
#include <pthread.h>

#define APPNAME "MobileAgentCPP"

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

JNIEnv *global_env;
jobject global_instance;
jobjectArray global_arguments;

void writeFile(const char* filePath, const char* fileContent);
void writeBinaryFile(const char* filePath,int size, const char* fileContent);

std::string RunV8(JNIEnv *env, jobject instance, jobjectArray arguments){
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

    __android_log_print(ANDROID_LOG_WARN, APPNAME, "The argument_count: %d", argument_count);
    __android_log_print(ANDROID_LOG_WARN, APPNAME, "printf: %s\n", argv[0]);

//    v8::V8::InitializeICUDefaultLocation(argv[0]);
//    v8::V8::InitializeExternalStartupData(argv[0]);
//    v8::V8::SetNativesDataBlob();

    v8::V8::InitializeICU();
    v8::Platform *platform = v8::platform::CreateDefaultPlatform();
    v8::V8::InitializePlatform(platform);

//    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
//    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();

    // Create a new Isolate and make it the current one.
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::SnapshotCreator snapshot;
    v8::Isolate* isolate = snapshot.GetIsolate();
//    v8::Isolate* isolate = v8::Isolate::New(create_params);
//    v8::SnapshotCreator snapshot(isolate, create_params.external_references);
    {
        v8::Isolate::Scope isolate_scope(isolate);
        // Create a stack-allocated handle scope.
        v8::HandleScope handle_scope(isolate);
        // Create a new context.
        v8::Local<v8::Context> context = v8::Context::New(isolate);
        // Enter the context for compiling and running the hello world script.
        v8::Context::Scope context_scope(context);
        // Create a string containing the JavaScript source code.
        v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, argv[0], v8::NewStringType::kNormal).ToLocalChecked();
        // Compile the source code.
        v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();
        // Run the script to get the result.
        v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
        // Convert the result to an UTF8 string and print it.
        v8::String::Utf8Value utf8(isolate, result);

        printf("%s\n", *utf8);

        __android_log_print(ANDROID_LOG_WARN, APPNAME, "printf: %s\n", *utf8);
        __android_log_print(ANDROID_LOG_WARN, APPNAME, "printf: %s\n", argv[0]);

        //Call to js function
        v8::Handle<v8::Object> global = context->Global();
        v8::Handle<v8::Value> function = global->Get(v8::String::NewFromUtf8(isolate, "addStr"));

        if ( function->IsFunction() )
        {
            v8::Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(function);
            int func_argc = 2;
            v8::Handle<v8::Value> func_argv[func_argc];
            func_argv[0] = v8::String::NewFromUtf8(isolate, "Hello");
            func_argv[1] = v8::String::NewFromUtf8(isolate, ", world!");
            v8::Handle<v8::Value> func_result = func->Call(global, func_argc, func_argv);

            v8::String::Utf8Value func_utf8(isolate, func_result);

            __android_log_print(ANDROID_LOG_WARN, APPNAME, "printf: %s\n", *func_utf8);
        } else {
            __android_log_print(ANDROID_LOG_WARN, APPNAME, "printf: %s\n", "Not a function");
        }

        snapshot.SetDefaultContext(context);
    }
    v8::StartupData startupDataBlob = snapshot.CreateBlob(v8::SnapshotCreator::FunctionCodeHandling::kKeep);
    __android_log_print(ANDROID_LOG_WARN, APPNAME, "printf: %d\n", startupDataBlob.raw_size);

    writeBinaryFile("/storage/3C99-9E99/FYP/jni.bin",startupDataBlob.raw_size,startupDataBlob.data);

    // Dispose the isolate and tear down V8.
//    isolate->Dispose();
//    v8::V8::Dispose();
//    v8::V8::ShutdownPlatform();
    delete create_params.array_buffer_allocator;
//    free(args_buffer);

    std::string hello = v8::V8::GetVersion();

    return hello;
}

std::string CallV8Func(JNIEnv *env, jobject instance, jobjectArray arguments){
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    if (!isolate) {
        __android_log_print(ANDROID_LOG_WARN, APPNAME, "Error: %s", "NULL isolate");
    } else {
        __android_log_print(ANDROID_LOG_WARN, APPNAME, "Error: %s", "not a NULL isolate");
    }

    std::string hello = "Hello from JNI activity C++ (CallV8Func)";
    return hello;
}

void *my_entry_function(void *params){
    __android_log_print(ANDROID_LOG_WARN, APPNAME, "Log: %s", "my_entry_function");
    RunV8(global_env,global_instance,global_arguments);
}

void start_thread() {
    int pt;
    pthread_t thread0;
//    int parameter = 1;
    pt = pthread_create(&thread0,NULL,&my_entry_function,NULL);
    pthread_join(thread0,NULL);
    if (pt!=0){
        __android_log_print(ANDROID_LOG_WARN, APPNAME, "Error: %s", "pthread_create");
    }
}

int getId() {
    pthread_t ptid = pthread_self();
    int threadId = 0;
    memcpy(&threadId,&ptid, sizeof(ptid));
    return threadId;
}

//void *my_entry_function(void *params){
//    __android_log_print(ANDROID_LOG_WARN, APPNAME, "Log: %s", "my_entry_function");
//}

void LoadSnapshotBlob() {
    __android_log_print(ANDROID_LOG_WARN, APPNAME, "LoadSnapshotBlob: %s", "here --- 1");
    jclass clazz = global_env->FindClass("com/exprograma/mobile/mobileagentcpp/JNI");

    if (clazz == nullptr) {
        __android_log_print(ANDROID_LOG_WARN, APPNAME, "LoadSnapshotBlob: %s", "clazz nullptr");
    }
    __android_log_print(ANDROID_LOG_WARN, APPNAME, "LoadSnapshotBlob: %s", "here --- 2");
    jmethodID constructor = global_env->GetMethodID(clazz,"<init>","()V");
    if (constructor == nullptr) {
        __android_log_print(ANDROID_LOG_WARN, APPNAME, "LoadSnapshotBlob: %s", "constructor nullptr");
    }

    jmethodID getLoc = global_env->GetMethodID(clazz,"getExternalStorageDir","()Ljava/lang/String;");
    if (getLoc == nullptr) {
        __android_log_print(ANDROID_LOG_WARN, APPNAME, "LoadSnapshotBlob: %s", "test nullptr");
    }

    __android_log_print(ANDROID_LOG_WARN, APPNAME, "LoadSnapshotBlob: %s", "here --- 3");
//    jobject result = global_env->CallObjectMethod(global_instance,test);
//    jobjectArray arr = global_env->NewObjectArray(2,global_env->FindClass("java/lang/String"),global_env->NewStringUTF("str"));
//    global_env->SetObjectArrayElement(arr,1,global_env->NewStringUTF("Next element"));

    jobject clazzObj = global_env->NewObject(clazz,constructor);
    jobject result = global_env->CallObjectMethod(clazzObj,getLoc);
//    global_env->DeleteLocalRef(arr);

    __android_log_print(ANDROID_LOG_WARN, APPNAME, "LoadSnapshotBlob: %s", "here --- 4");
    const char* str = global_env->GetStringUTFChars((jstring) result,NULL);
    __android_log_print(ANDROID_LOG_WARN, APPNAME, "LoadSnapshotBlob: %s", str);

    writeFile("/storage/3C99-9E99/FYP/jni.txt","hello world");
}

void writeFile(const char* filePath, const char* fileContent) {
    FILE* file = fopen(filePath,"w+");
    if (file != NULL) {
        fputs(fileContent,file);
        fflush(file);
        fclose(file);
        __android_log_print(ANDROID_LOG_WARN, APPNAME, "Log: %s to :%s", "File write success", filePath);
    } else {
        __android_log_print(ANDROID_LOG_WARN, APPNAME, "Error: %s", "File write fail");
        __android_log_print(ANDROID_LOG_WARN, APPNAME, "Error: %d: %s", errno,strerror(errno));
    }
}

void writeBinaryFile(const char* filePath,int size, const char* fileContent) {
//    char buffer[size];
    std::ofstream file(filePath,std::ios::out | std::ios::binary);
    file.write(fileContent,size);
    file.close();
    __android_log_print(ANDROID_LOG_WARN, APPNAME, "Log: %s to :%s", "File write success", filePath);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_exprograma_mobile_mobileagentcpp_MainActivity_runNativeV8(JNIEnv *env, jobject instance, jobjectArray arguments) {

//    global_env = env;
//    global_instance = instance;
//    global_arguments = arguments;

//    LoadSnapshotBlob();

//    start_thread();
//    __android_log_print(ANDROID_LOG_WARN, APPNAME, "getId: %d", getId());

    std::string hello = "Hello from JNI activity C++";
    try {
        hello = RunV8(env,instance,arguments);
    } catch (const char* msg) {
        __android_log_print(ANDROID_LOG_WARN, APPNAME, "Error: %s", msg);
    } catch (std::exception& e) {
        __android_log_print(ANDROID_LOG_WARN, APPNAME, "Error(std::exception): %s", e.what());
    }

    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_exprograma_mobile_mobileagentcpp_MainActivity_callV8Func(JNIEnv *env, jobject instance, jobjectArray arguments) {

    std::string hello = "Hello from JNI activity C++ (JNI_callV8Func)";
    try {
        hello = CallV8Func(env,instance,arguments);
    } catch (const char* msg) {
        __android_log_print(ANDROID_LOG_WARN, APPNAME, "Error: %s", msg);
    } catch (std::exception& e) {
        __android_log_print(ANDROID_LOG_WARN, APPNAME, "Error(std::exception): %s", e.what());
    }

    return env->NewStringUTF(hello.c_str());
}
