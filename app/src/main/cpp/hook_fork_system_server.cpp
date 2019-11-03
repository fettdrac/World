//
// Created by Administrator on 2019/11/2.
//

#include <jni.h>
#include <unistd.h>
#include "hook_fork_system_server.h"
#include "Hook.h"
#include "log_utils.h"
#include "jni_utils.h"
#include "method_utils.h"
#include "ServiceManager.h"
#include "process_utils.h"

//com.android.internal.os.ZygoteInit#forkSystemServer

//com.android.internal.os.ZygoteInit#handleSystemServerProcess
#define METHOD_NAME "handleSystemServerProcess"
jobject my_handle_system_server_process(JNIEnv *env,jclass def_class,jobject parsedArgs) {
    AKLog("invoked on pid:%d\n", getpid());
    jmethodID old_method = Hook::GetInstance()->GetOldMethod(METHOD_NAME);
    if (!old_method) {
        AKLogErr("fail to get old_method\n");
        return nullptr;
    }

    bool result = ServiceManager::LoadService(env, "/data/system",true);
    AKLogErr("LoadService:%d\n", result);
    //调用原函数
    return env->CallStaticObjectMethod(def_class, old_method, parsedArgs);
}


void hook_fork_system_server(JNIEnv *env){
    jclass zygote_init_class=env->FindClass("com/android/internal/os/ZygoteInit");
    //private static Runnable handleSystemServerProcess(ZygoteConnection.Arguments parsedArgs)
    jmethodID handle_system_server_process=get_method_by_name(env,zygote_init_class,METHOD_NAME);
    if(!handle_system_server_process){
        try_catch(env,true);
        return;
    }
    Hook::GetInstance()->JavaHook(env,METHOD_NAME,handle_system_server_process,
            reinterpret_cast<void*>(my_handle_system_server_process));
    //DEMO
    jmethodID old_method= Hook::GetInstance()->GetOldMethod(METHOD_NAME);
    AKLog("old_method from map:%p\n",old_method);
}