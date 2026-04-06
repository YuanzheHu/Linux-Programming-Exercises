#include "file_analyzer.h"

#include <cerrno>
#include <cstring>
#include <iostream>

#include <fcntl.h>
#include <unistd.h>

#include "language.h"
#include "line_classifier.h"

bool analyzeFile(const std::string& filepath, FileStats& result)
{
    int fd = open(filepath.c_str(), O_RDONLY);
    if (fd == -1)
    {
        std::cerr << "Error opening file " << filepath << ": " << strerror(errno) << std::endl;
        return false;
    }

    result.filepath = filepath;
    result.language = detectLanguage(filepath);

    const size_t buffer_size = 4096;
    char buffer[buffer_size];
    std::string current_line;
    bool in_block_comment = false;
    ssize_t n;

    while ((n = read(fd, buffer, buffer_size)) > 0)
    {
        for (ssize_t i = 0; i < n; ++i)
        {
            if (buffer[i] == '\n')
            {
                result.total_lines++;

                LineType type = classifyLine(current_line, in_block_comment);
                switch (type)
                {
                    case LineType::CODE:
                        result.code_lines++;
                        break;
                    case LineType::COMMENT:
                        result.comment_lines++;
                        break;
                    case LineType::BLANK:
                        result.blank_lines++;
                        break;
                }
                current_line.clear();
            }
            else
            {
                current_line += buffer[i];
            }
        }
    }

    if (!current_line.empty())
    {
        result.total_lines++;
        LineType type = classifyLine(current_line, in_block_comment);
        switch (type)
        {
            case LineType::CODE:
                result.code_lines++;
                break;
            case LineType::COMMENT:
                result.comment_lines++;
                break;
            case LineType::BLANK:
                result.blank_lines++;
                break;
        }
    }

    if (n == -1)
    {
        std::cerr << "Error reading file " << filepath << ": " << strerror(errno) << std::endl;
        close(fd);
        return false;
    }

    close(fd);
    return true;
}
