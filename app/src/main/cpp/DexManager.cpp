//
// Created by Administrator on 2019/11/2.
//

#include "DexManager.h"
#include "context_utils.h"
#include "classloader_utils.h"
#include "jni_utils.h"
#include "log_utils.h"
#include "AndHook.h"

jobject DexManager::NewLoader(JNIEnv *env,const std::string &alias,
        const std::string &dex_path, const std::string &opt_dir,
                           const std::string &lib_dir) {
    jclass dcl_class = env->FindClass("dalvik/system/DexClassLoader");
    //public DexClassLoader(String dexPath, String optimizedDirectory, String librarySearchPath, ClassLoader parent)
    jmethodID constructor = env->GetMethodID(dcl_class, "<init>",
                                             "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/ClassLoader;)V");
    AKLog("constructor:%p\n",constructor);
    jstring j_lib_dir = nullptr;
    if (!lib_dir.empty()) {
        j_lib_dir = env->NewStringUTF(lib_dir.c_str());
    }
    //java.lang.ClassLoader.getSystemClassLoader
    jobject sys_cl=get_system_classloader(env);
    jobject dcl = env->NewObject(dcl_class, constructor,
                                 env->NewStringUTF(dex_path.c_str()),
                                 env->NewStringUTF(opt_dir.c_str()),
                                 j_lib_dir,
                                 sys_cl);
    //AKEnableFastDexLoad(true);
    //jobject dcl=AKLoadFileDex(env,dex_path.c_str(),opt_dir.c_str(), nullptr,sys_cl);
    AKLog("dcl:%p\n",dcl);
    if(!dcl){
        try_catch(env,true);
        return nullptr;
    }
    jobject global_gcl_ = env->NewGlobalRef(dcl);
    path_dex_map_.insert(std::make_pair(alias, global_gcl_));
    return global_gcl_;
}
