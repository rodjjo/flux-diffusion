#include "include/dfe/config.h"

#include <unistd.h>
#include <limits.h>

namespace dfe
{
namespace config
{
    

namespace 
{
    std::wstring exe_path;
}  

std::wstring executable_path() {
    if (exe_path.empty()) {
        char buffer[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (len != -1) {
            buffer[len] = '\0';
            exe_path = std::wstring(buffer, buffer + len);
        }
    }
    return exe_path;
}

std::wstring executable_directory() {
    std::wstring path = executable_path();
    size_t pos = path.find_last_of(L"/\\");
    if (pos != std::wstring::npos) {
        return path.substr(0, pos) + L"/";
    }
    return L"./";
}

std::wstring python_venv_directory() {
    return executable_directory() + L"venv/";
}

std::wstring python_executable_path() {
    return python_venv_directory() + L"bin/python3";
}

std::wstring entrypoint_directory() {
    return executable_directory() + L"python_stuff/";
}

std::wstring entrypoint_script() {
    return entrypoint_directory() + L"entrypoint.py";
}

std::wstring depedencies_script() {
    return entrypoint_directory() + L"dependencies.py";
}

std::wstring filename(const std::wstring& path) {
    size_t pos = path.find_last_of(L"/\\");
    if (pos != std::wstring::npos) {
        return path.substr(pos + 1);
    }
    return path;
}

std::string entrypoint_module() {
    std::wstring path = filename(entrypoint_script());
    size_t pos = path.find_last_of(L".");
    return std::string(path.begin(), path.end()).substr(0, pos);
}

std::string dependencies_module() {
    std::wstring path = filename(depedencies_script());
    size_t pos = path.find_last_of(L".");
    return std::string(path.begin(), path.end()).substr(0, pos);
}

} // namespace config    
} // namespace dfe
