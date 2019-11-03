//
// Created by Administrator on 2019/11/2.
//

#ifndef WORLD_SERVICEMANAGER_H
#define WORLD_SERVICEMANAGER_H


#include <jni.h>
#include <map>

#define DEX_ALIAS "DexService"

jobject get_service_impl(JNIEnv *env,const std::string &name_str);

void connect_in_apps(JNIEnv *env);

class ServiceManager {
public:
    static ServiceManager* GetInstance(){
        static ServiceManager *instance=new ServiceManager();
        return instance;
    }

    static bool LoadService(JNIEnv *env,const std::string &opt_dir,bool as_server);

    jobject GetService(JNIEnv *env,const std::string &name_str);

    bool ConnectService(JNIEnv *env);

private:
    std::map<std::string,jobject> name_service_map_;
};


#endif //WORLD_SERVICEMANAGER_H
