#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <string>

// Reads file metadata via stat() and prints it to stdout.
// filepath: target path to inspect.
// verbose : when true, prints inode/owner/link details.
// format  : "normal" for text output, "json" for JSON-like output.
void showFileInfo(const std::string& filepath, bool verbose, const std::string& format);

#endif
