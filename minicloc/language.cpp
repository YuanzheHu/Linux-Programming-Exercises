#include "language.h"

#include <algorithm>
#include <cctype>
#include <map>

std::string getExtention(const std::string& filename)
{
    size_t pos = filename.rfind('.');
    if (pos == std::string::npos) return "";
    std::string ext = filename.substr(pos + 1);

    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char character) {
        return static_cast<char>(std::tolower(character));
    });
    return ext;
}

std::string detectLanguage(const std::string& filepath)
{
    static const std::map<std::string, std::string> ext_map = {
        {"c", "C"},
        {"h", "C/C++ Header"},
        {"cpp", "C++"},
        {"cxx", "C++"},
        {"cc", "C++"},
        {"hpp", "C/C++ Header"},
        {"hxx", "C/C++ Header"},
        {"py", "Python"},
        {"js", "JavaScript"},
        {"ts", "TypeScript"},
        {"java", "Java"},
        {"go", "Go"},
        {"rs", "Rust"},
        {"sh", "Shell"},
        {"bash", "Shell"},
        {"md", "Markdown"},
        {"json", "JSON"},
        {"yaml", "YAML"},
        {"yml", "YAML"},
        {"xml", "XML"},
        {"sql", "SQL"},
        {"css", "CSS"},
        {"html", "HTML"},
        {"htm", "HTML"},
    };

    std::string ext = getExtention(filepath);
    auto it = ext_map.find(ext);
    if (it != ext_map.end())
    {
        return it->second;
    }
    return "Unknown";
}
