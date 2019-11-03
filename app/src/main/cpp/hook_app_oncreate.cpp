//
// Created by Administrator on 2019/11/2.
//

#include <jni.h>
#include <unistd.h>
#include "hook_app_oncreate.h"
#include "Hook.h"
#include "log_utils.h"
#include "process_utils.h"
#include "ServiceManager.h"
#include "context_utils.h"

#define METHOD_NAME "callApplicationOnCreate"

void my_call_app_oncreate(JNIEnv *env,jobject thiz,
        jobject app){
    jmethodID old_method= Hook::GetInstance()->GetOldMethod(METHOD_NAME);
    if(!old_method){
        AKLogErr("fail to get old_method\n");
        return;
    }
    std::string process_name=get_process_name();
    AKLog("process_name:%s\n",process_name.c_str());

    //连接到远程服务
    connect_in_apps(env);

    //调用原函数
    env->CallVoidMethod(thiz,old_method,app);
}

void hook_call_app_oncreate(JNIEnv *env){
    jclass in_class=env->FindClass("android/app/Instrumentation");
    //android.app.Instrumentation.callApplicationOnCreate(Application app)->V
    jmethodID call_app_on_create=env->GetMethodID(in_class,METHOD_NAME,
            "(Landroid/app/Application;)V");
    AKLog("method:%p\n",call_app_on_create);
    Hook::GetInstance()->JavaHook(env,METHOD_NAME,call_app_on_create, reinterpret_cast<void*>(my_call_app_oncreate));
    //DEMO
    jmethodID old_method= Hook::GetInstance()->GetOldMethod(METHOD_NAME);
    AKLog("old_method from map:%p\n",old_method);
}
