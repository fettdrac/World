//
// Created by Administrator on 2019/11/2.
//

#include <unistd.h>
#include "context_utils.h"
#include "log_utils.h"
#include "jni_utils.h"

jobject get_app_context(JNIEnv *env) {
    static jobject context= nullptr;
    if(!context) {
        jclass app_thread_class = env->FindClass("android/app/ActivityThread");
        //android.app.ActivityThread#currentApplication()->android.app.Application
        jmethodID get_app = env->GetStaticMethodID(app_thread_class, "currentApplication",
                                                   "()Landroid/app/Application;");
        jobject app = env->CallStaticObjectMethod(app_thread_class, get_app);
        if (try_catch(env, true)) {
            AKLogErr("this process:%d has not context\n", getpid());
            return nullptr;
        }
        AKLog("context:%p\n",app);
        context = env->NewGlobalRef(app);
    }
    return context;
}

std::string get_abs_path(JNIEnv *env,jobject file){
    jclass file_class=env->FindClass("java/io/File");
    //public String getAbsolutePath()
    jmethodID get_abs_path=env->GetMethodID(file_class,"getAbsolutePath","()Ljava/lang/String;");
    jstring j_path=(jstring)env->CallObjectMethod(file,get_abs_path);
    std::string path_str=env->GetStringUTFChars(j_path, nullptr);
    return path_str;
}

bool get_dir_normal(JNIEnv *env, const std::string &name_str,std::string &dir_path) {
    jobject context_=get_app_context(env);
    if(!context_)
        return false;
    jclass context_wrapper_class=env->FindClass("android/content/ContextWrapper");
    //public File getDir(String name, int mode)
    jmethodID get_dir=env->GetMethodID(context_wrapper_class,"getDir","(Ljava/lang/String;I)Ljava/io/File;");
    jobject dir=env->CallObjectMethod(context_,get_dir,
            env->NewStringUTF(name_str.c_str()),0);
    AKLog("dir object:%p\n",dir);
    if(!dir){
        try_catch(env,true);
        return false;
    }
    dir_path=get_abs_path(env,dir);
    AKLog("dir:%s path:%s\n",name_str.c_str(),dir_path.c_str());
    return true;
}
