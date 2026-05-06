#include "utils.hpp"

#ifndef _WIN32
#include <linux/limits.h>
#endif

float frandom() {
    return (float)(rand() % 65536) / 65536.0f;
}

std::string rt_abs_path(const char* path) {
#ifdef _WIN32
    char _file_path_buffer[_MAX_PATH];
    ASSERT(_fullpath(_file_path_buffer, path, _MAX_PATH), "Invalid file path");
#else
    char _file_path_buffer[PATH_MAX + 1];
    ASSERT(realpath(path, _file_path_buffer), "Invalid file path");
#endif
    return std::string(_file_path_buffer);
}
