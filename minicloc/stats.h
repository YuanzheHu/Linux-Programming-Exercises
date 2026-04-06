#pragma once

#include <string>

struct FileStats
{
    std::string filepath;
    std::string language;
    int total_lines = 0;
    int code_lines = 0;
    int comment_lines = 0;
    int blank_lines = 0;
};

struct LangStats
{
    std::string language;
    int files = 0;
    int total_lines = 0;
    int code_lines = 0;
    int comment_lines = 0;
    int blank_lines = 0;

    void merge(const FileStats& fs)
    {
        files++;
        total_lines += fs.total_lines;
        code_lines += fs.code_lines;
        comment_lines += fs.comment_lines;
        blank_lines += fs.blank_lines;
    }
};
