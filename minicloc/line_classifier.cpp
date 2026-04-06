#include "line_classifier.h"

std::string trim(const std::string& str)
{
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

LineType classifyLine(const std::string& line, bool& in_block_comment)
{
    std::string t = trim(line);

    if (t.empty()) return LineType::BLANK;

    if (in_block_comment)
    {
        if (t.find("*/") != std::string::npos)
        {
            in_block_comment = false;
            size_t end_pos = t.find("*/") + 2;
            std::string after = trim(t.substr(end_pos));
            return after.empty() ? LineType::COMMENT : LineType::CODE;
        }
        return LineType::COMMENT;
    }

    if (t.substr(0, 2) == "//") return LineType::COMMENT;

    if (t.substr(0, 2) == "/*")
    {
        if (t.find("*/") != std::string::npos)
        {
            return LineType::CODE;
        }
        in_block_comment = true;
        return LineType::COMMENT;
    }

    if (t.substr(0, 1) == "#") return LineType::COMMENT;
    return LineType::CODE;
}
