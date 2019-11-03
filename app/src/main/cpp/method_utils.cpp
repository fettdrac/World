//
// Created by Administrator on 2019/11/2.
//

#include "method_utils.h"
#include "log_utils.h"

bool is_static(JNIEnv *env,jobject j_method){
    jclass method_class=env->FindClass("java/lang/reflect/Method");
    //public int getModifiers()
    jmethodID get_modifiers=env->GetMethodID(method_class,"getModifiers","()I");
    int modifiers=env->CallIntMethod(j_method,get_modifiers);
    //java.lang.reflect.Modifier.isStatic(I) -> bool (static)
    jclass modifier_class=env->FindClass("java/lang/reflect/Modifier");
    jmethodID is_method_static=env->GetStaticMethodID(modifier_class,"isStatic","(I)Z");
    return env->CallBooleanMethod(j_method,is_method_static,
            modifiers);
}

jmethodID get_method_by_name(JNIEnv *env, jclass def_class, const std::string &name) {
    jclass cls_class=env->FindClass("java/lang/Class");
    // public Method[] getDeclaredMethods() throws SecurityException
    jmethodID get_declared_methods=env->GetMethodID(cls_class,"getDeclaredMethods","()[Ljava/lang/reflect/Method;");
    jobjectArray methods=(jobjectArray)env->CallObjectMethod(def_class,get_declared_methods);
    int length=env->GetArrayLength(methods);

    jclass method_class=env->FindClass("java/lang/reflect/Method");
    // public String getName()
    jmethodID get_method_name=env->GetMethodID(method_class,"getName","()Ljava/lang/String;");

    jmethodID current_method= nullptr;
    for (int i = 0; i < length; ++i) {
        jobject method_element=env->GetObjectArrayElement(methods,i);

        jstring j_method_name=(jstring)env->CallObjectMethod(method_element,get_method_name);
        const char* method_name=env->GetStringUTFChars(j_method_name,nullptr);
        if(name==method_name) {
            //bool is_method_static=is_static(env,method_element);
            current_method=env->FromReflectedMethod(method_element);
            break;
        }

        env->ReleaseStringUTFChars(j_method_name,method_name);
        env->DeleteLocalRef(j_method_name);
        env->DeleteLocalRef(method_element);
    }
    AKLog("%s method pointer:%p\n",name.c_str(),current_method);
    return current_method;
}
