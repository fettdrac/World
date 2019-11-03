//
// Created by Administrator on 2019/11/2.
//

#include <dirent.h>
#include <jni.h>
#include <sys/stat.h>
#include <unistd.h>
#include "file_utils.h"
#include "log_utils.h"
#include "shell_utils.h"

bool dir_exist(const std::string &path) {
    DIR* dir=opendir(path.c_str());
    bool exist=dir!= nullptr;
    if(dir)
        closedir(dir);
    return exist;
}

bool dir_writable(const std::string &path_str){
    const char* path=path_str.c_str();
    DIR* dir=opendir(path);
    if(!dir)//限定这是一个目录
        return false;
    closedir(dir);
    return generic_writable(path_str);
}


#define MODE_0777 (S_IRWXU | S_IRWXG | S_IRWXO)

#define MODE_0744 (S_IRWXU | S_IRGRP | S_IROTH)

bool get_or_create_dir(const std::string &path) {
    if(dir_writable(path))
        return true;
    return mkdir(path.c_str(),MODE_0744)==JNI_OK;
}

bool access_int_to_bool(const std::string &path,int mode){
    return access(path.c_str(),mode)==JNI_OK;
}

bool generic_writable(const std::string &path_str) {
    return access_int_to_bool(path_str,W_OK);
}

int read_text_file(const std::string &path,std::string &result){
   std::string cmd=std::string("cat").append(" ").append(path);
    AKLog("cmd:%s\n",cmd.c_str());
    return shell_with_return(cmd,result)==JNI_OK;
}

bool generic_exist(const std::string &path) {
    return access_int_to_bool(path,(F_OK | R_OK));
}
