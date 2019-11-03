//
// Created by Administrator on 2019/11/2.
//

#include "classloader_utils.h"
#include "log_utils.h"
#include "jni_utils.h"

jobject get_system_classloader(JNIEnv *env) {
    static jobject sys_classloader_=nullptr;
    if(!sys_classloader_) {
        jclass cl_class = env->FindClass("java/lang/ClassLoader");
        //java.lang.ClassLoader.getSystemClassLoader
        jmethodID get_sys_cl = env->GetStaticMethodID(cl_class, "getSystemClassLoader",
                                                      "()Ljava/lang/ClassLoader;");
        jobject sys_cl = env->CallStaticObjectMethod(cl_class, get_sys_cl);
        AKLog("sys_cl:%p\n", sys_cl);
        sys_classloader_ = env->NewGlobalRef(sys_cl);
    }
    return sys_classloader_;
}

jclass load_by_specific(JNIEnv *env, jobject cl, const std::string &name) {
    jclass cl_class = env->FindClass("java/lang/ClassLoader");
    //public Class<?> loadClass(String name) throws ClassNotFoundException -> java.lang.Class
    jmethodID load_class=env->GetMethodID(cl_class,"loadClass","(Ljava/lang/String;)Ljava/lang/Class;");
    jclass clazz=(jclass) env->CallObjectMethod(cl,load_class,
            env->NewStringUTF(name.c_str()));
    if(try_catch(env,true))
        return nullptr;
    AKLog("clazz:%p\n",clazz);
    return  (jclass)env->NewGlobalRef(clazz);
}
