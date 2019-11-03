//
// Created by Administrator on 2019/11/2.
//

#ifndef WORLD_FILE_UTILS_H
#define WORLD_FILE_UTILS_H

#include <string>

bool dir_exist(const std::string &path);
bool dir_writable(const std::string &path_str);
bool generic_writable(const std::string &path_str);
bool generic_exist(const std::string &path);
bool get_or_create_dir(const std::string &path);
int read_text_file(const std::string &path,std::string &result);
#endif //WORLD_FILE_UTILS_H
