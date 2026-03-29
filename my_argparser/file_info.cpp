#include "file_info.h"

#include <ctime>
#include <iostream>
#include <string>
#include <sys/stat.h>

namespace
{
// Convert Unix permission bits into rwx text, e.g. "rwxr-xr--".
std::string permToString(mode_t mode)
{
    std::string result = "---------";

    if (mode & S_IRUSR) result[0] = 'r';
    if (mode & S_IWUSR) result[1] = 'w';
    if (mode & S_IXUSR) result[2] = 'x';

    if (mode & S_IRGRP) result[3] = 'r';
    if (mode & S_IWGRP) result[4] = 'w';
    if (mode & S_IXGRP) result[5] = 'x';

    if (mode & S_IROTH) result[6] = 'r';
    if (mode & S_IWOTH) result[7] = 'w';
    if (mode & S_IXOTH) result[8] = 'x';

    return result;
}
}

// Query file metadata and print in either human-readable or JSON format.
void showFileInfo(const std::string& filepath, bool verbose, const std::string& format)
{
    struct stat file_stat;

    // stat() fills file_stat; non-zero means an access/query error.
    if (stat(filepath.c_str(), &file_stat) != 0)
    {
        std::cerr << "Error: Cannot access file: " << filepath << std::endl;
        return;
    }

    // Convert epoch modification time to local date-time string.
    char time_buf[64];
    struct tm* tm_info = localtime(&file_stat.st_mtime);
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);

    // Classify basic file type from st_mode.
    std::string file_type;
    if (S_ISREG(file_stat.st_mode))             file_type = "Regular File";
    else if (S_ISDIR(file_stat.st_mode))        file_type = "Directory";
    else if (S_ISLNK(file_stat.st_mode))        file_type = "Symbolic Link";
    else                                        file_type = "Other";

    // JSON branch: machine-friendly output for scripting.
    if (format == "json")
    {
        std::cout << "{\n";
        std::cout << "  \"path\": \"" << filepath << "\",\n";
        std::cout << "  \"type\": \"" << file_type << "\",\n";
        std::cout << "  \"size\": " << file_stat.st_size << ",\n";
        std::cout << "  \"permissions\": \"" << permToString(file_stat.st_mode) << "\",\n";
        std::cout << "  \"last_modified\": \"" << time_buf << "\"";
        if (verbose)
        {
            // Append extended metadata only in verbose mode.
            std::cout << ",\n";
            std::cout << "  \"inode\": " << file_stat.st_ino << ",\n";
            std::cout << "  \"hard_links\": " << file_stat.st_nlink << ",\n";
            std::cout << "  \"owner_uid\": " << file_stat.st_uid << ",\n";
            std::cout << "  \"owner_gid\": " << file_stat.st_gid << "\n";
        }
        else
        {
            std::cout << "\n";
        }
        std::cout << "}\n";
    }
    // Text branch: human-readable layout for terminal viewing.
    else
    {
        std::cout << "============File Information============" << std::endl;
        std::cout << "Path: " << filepath << std::endl;
        std::cout << "Type: " << file_type << std::endl;
        std::cout << "Size: " << file_stat.st_size << " bytes" << std::endl;
        std::cout << "Permissions: " << permToString(file_stat.st_mode) << std::endl;
        std::cout << "Last Modified: " << time_buf << std::endl;
        if (verbose)
        {
            std::cout << "Inode: " << file_stat.st_ino << std::endl;
            std::cout << "Hard Links: " << file_stat.st_nlink << std::endl;
            std::cout << "Owner UID: " << file_stat.st_uid << std::endl;
            std::cout << "Owner GID: " << file_stat.st_gid << std::endl;
        }
        std::cout << "========================================" << std::endl;
    }
}
