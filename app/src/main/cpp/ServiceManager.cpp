//
// Created by Administrator on 2019/11/2.
//

#include <string>
#include <unistd.h>
#include "ServiceManager.h"
#include "file_utils.h"
#include "DexManager.h"
#include "log_utils.h"
#include "classloader_utils.h"
#include "jni_utils.h"
#include "context_utils.h"

#define DEFAULT_SERVICE_DEX_PATH "/system/DexService.dex"

static std::string service_dex_path_;

bool add_to_service_manager(JNIEnv *env,jobject dcl){
    jclass service_class= load_by_specific(env,dcl,"com.pvdnc.world.DexService"/*java层加载，点号分隔*/);
    if(!service_class)
        return false;
    AKLog("attempt to load service\n");
    //com.pvdnc.world.DexService.systemReady(Context context) -> V
    jmethodID system_ready=env->GetStaticMethodID(service_class,"systemReady","(Landroid/content/Context;)V");
    if(!system_ready){
        return !try_catch(env,true);
    }
    env->CallStaticVoidMethod(service_class,system_ready,
                              nullptr/*用ServiceManager加载*/);
    AKLog("system_ready:%p has been called\n",system_ready);
    return !try_catch(env,true);
}

bool connect_to_de_service(JNIEnv *env,jobject dcl) {
    jobject remote = get_service_impl(env, DEX_ALIAS);
    if (!remote) {
        AKLogErr("service has not been prepared\n");
        return false;
    }
    //加载客户端代理类
    jclass stub_class = load_by_specific(env, dcl,
                                         "com.pvdnc.world.IDexService$Stub"/*java层加载，点号分隔*/);
    if (!stub_class)
        return false;
    //com.pvdnc.world.IDexService.Stub.asInterface
    jmethodID as_interface = env->GetStaticMethodID(stub_class, "asInterface",
                                                    "(Landroid/os/IBinder;)Lcom/pvdnc/world/IDexService;");

    jobject proxy = env->CallStaticObjectMethod(stub_class, as_interface, remote);
    jclass proxy_class = env->GetObjectClass(proxy);
    jmethodID add_client = env->GetMethodID(proxy_class, "addClient", "(Landroid/os/IBinder;)V");

    //加载服务类
    jclass service_class = load_by_specific(env, dcl, "com.pvdnc.world.DexService"/*java层加载，点号分隔*/);
    if (!service_class)
        return false;
    jmethodID system_ready = env->GetStaticMethodID(service_class, "systemReady",
                                                    "(Landroid/content/Context;)V");
    if (!system_ready) {
        return !try_catch(env, true);
    }
    env->CallStaticVoidMethod(service_class, system_ready,
                              nullptr);//初始化本地Caller
    jmethodID get_server_instance = env->GetStaticMethodID(service_class, "get",
                                                           "()Lcom/pvdnc/world/DexService;");
    jobject server_instance = env->CallStaticObjectMethod(service_class, get_server_instance);
    AKLog("server_instance:%p\n", server_instance);
    if (!server_instance) {
        AKLogErr("unknown systemReady result\n");
        return false;
    }
    //向服务端报告自身
    env->CallVoidMethod(proxy, add_client, server_instance);
    return !try_catch(env, true);
}

bool ServiceManager::LoadService(JNIEnv *env,const std::string &opt_dir,
        bool as_server) {
    //检查Dex
    if(service_dex_path_.empty()) {
        service_dex_path_ = DEFAULT_SERVICE_DEX_PATH;
    }
    AKLog("service_dex_path:%s\n",service_dex_path_.c_str());
    if(!generic_exist(service_dex_path_)) {
        AKLogErr("dex:%s does not exist\n",service_dex_path_.c_str());
        return false;
    }
    //加载DexService类
    jobject dcl= DexManager::GetInstance()->NewLoader(env,DEX_ALIAS,
            service_dex_path_,
            opt_dir,"");
    if(!dcl) {
        AKLogErr("dcl is nullptr ,please check NewLoader\n");
        return false;
    }
    if(as_server) {
        //加载Binder服务
        return add_to_service_manager(env, dcl);
    }else{
        //连接到Binder服务
        return connect_to_de_service(env,dcl);
    }
}

jobject get_service_impl(JNIEnv *env,const std::string &name_str){
    const char *name = name_str.c_str();
    jclass service_manager_class = env->FindClass("android/os/ServiceManager");
    //android.os.ServiceManager#getService(String name_str) -> IBinder
    jmethodID get_service = env->GetStaticMethodID(service_manager_class, "getService",
                                                   "(Ljava/lang/String;)Landroid/os/IBinder;");
    jobject remote = env->CallStaticObjectMethod(service_manager_class, get_service,
                                                 env->NewStringUTF(name));
    AKLog("%s remote:%p\n", name, remote);
    if (!remote) {
        try_catch(env, true);
        return nullptr;
    }
    return env->NewGlobalRef(remote);
}

void connect_in_apps(JNIEnv *env) {
    AKLog("connect to service in pid:%d\n",getpid());

    std::string opt_dir_str;
    if(!get_dir_normal(env,"optDex",opt_dir_str)){
        AKLogErr("fail to get opt_dir in pid:%d\n",getpid());
        return;
    }
    bool result=ServiceManager::LoadService(env,opt_dir_str, false);
    AKLog("LoadService result:%d\n",result);
}

jobject ServiceManager::GetService(JNIEnv *env, const std::string &name_str){
    auto cached_item= name_service_map_.find(name_str);
    jobject global_remote_= nullptr;
    if(cached_item==name_service_map_.end()) {
        AKLog("need to get from ServiceManager\n");
        global_remote_=get_service_impl(env,name_str);
        name_service_map_.insert(std::make_pair(name_str, global_remote_));
    }else {
        global_remote_ = cached_item->second;
    }
    AKLog("%s remote:%p\n", name_str.c_str(), global_remote_);
    return global_remote_;
}



bool ServiceManager::ConnectService(JNIEnv *env) {
    jobject remote= GetService(env,DEX_ALIAS);

    return false;
}
