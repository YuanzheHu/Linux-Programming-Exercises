#pragma once

#include <string>

enum class LineType
{
    CODE,
    COMMENT,
    BLANK
};

std::string trim(const std::string& str);
LineType classifyLine(const std::string& line, bool& in_block_comment);
