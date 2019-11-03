//
// Created by Administrator on 2019/11/2.
//

#ifndef WORLD_DEXMANAGER_H
#define WORLD_DEXMANAGER_H

#include <map>
#include <string>
#include <jni.h>

class DexManager {
public:
    static DexManager* GetInstance(){
        static DexManager *instance=new DexManager();
        return instance;
    }

    jobject NewLoader(JNIEnv *env,const std::string &alias,
            const std::string &dex_path, const std::string &opt_dir,
            const std::string &lib_dir);

private:
    std::map<std::string,jobject> path_dex_map_;
};


#endif //WORLD_DEXMANAGER_H
