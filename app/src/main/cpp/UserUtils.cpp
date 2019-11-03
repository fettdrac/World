//
// Created by Administrator on 2019/11/2.
//

#include <unistd.h>
#include "UserUtils.h"
#include "file_utils.h"
#include "process_utils.h"

bool UserUtils::GetCacheDir(JNIEnv *env,std::string &path) {
    if (getuid() == 0
        || get_process_name() == "system_server") {
        path = "/data/system";
        return get_or_create_dir(path);
    }
    return false;
}
