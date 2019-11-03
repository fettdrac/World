//
// Created by Administrator on 2019/11/2.
//

#ifndef WORLD_HOOK_H
#define WORLD_HOOK_H

#include <map>
#include <string>
#include <jni.h>

class Hook {
public:
    static Hook* GetInstance(){
        static Hook* instance=new Hook();
        return instance;
    }

    bool InitJavaHook(JavaVM *vm,JNIEnv *env);

    Hook();

    void NativeHook(const std::string& name,void* symbol,void* override_func);

    void* GetOldFunction(const std::string &name);

    void JavaHook(JNIEnv *env,const std::string &name, jmethodID target,void* override_func);

    jmethodID GetOldMethod(const std::string &name);

private:
    std::map<std::string,void*> native_func_map_;

    bool java_hookable_;
    std::map<std::string,jmethodID> java_func_map_;
};


#endif //WORLD_HOOK_H
