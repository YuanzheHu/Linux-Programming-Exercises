#include "file_collector.h"

#include <cerrno>
#include <cstring>
#include <iostream>

#include <dirent.h>
#include <sys/stat.h>

#include "language.h"

void collectFiles(const std::string& path, const std::vector<std::string>& filter_exts, std::vector<std::string>& out_files)
{
    struct stat st;
    if (stat(path.c_str(), &st) == -1)
    {
        std::cerr << "Error accessing " << path << ": " << strerror(errno) << std::endl;
        return;
    }

    if (S_ISREG(st.st_mode))
    {
        std::string lang = detectLanguage(path);
        if (lang.empty()) return;

        if (!filter_exts.empty())
        {
            std::string ext = getExtention(path);
            bool match = false;
            for (const std::string& fe : filter_exts)
            {
                if (ext == fe)
                {
                    match = true;
                    break;
                }
            }
            if (!match) return;
        }
        out_files.push_back(path);
        return;
    }

    if (!S_ISDIR(st.st_mode))
    {
        std::cerr << "Skipping non-regular, non-directory: " << path << std::endl;
        return;
    }

    DIR* dir = opendir(path.c_str());
    if (!dir)
    {
        std::cerr << "Error opening directory " << path << ": " << strerror(errno) << std::endl;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        std::string name = entry->d_name;

        if (name == "." || name == "..") continue;
        if (name[0] == '.') continue;
        if (name == "node_modules" || name == ".git" || name == "build" || name == "cmake-build-debug") continue;

        std::string full_path = path + "/" + name;
        collectFiles(full_path, filter_exts, out_files);
    }
    closedir(dir);
}
