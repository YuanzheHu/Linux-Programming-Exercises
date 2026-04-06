#include "report.h"

#include <algorithm>
#include <cstdio>
#include <iostream>

void printReport(const std::map<std::string, LangStats>& lang_stats, const std::vector<FileStats>& all_files, bool show_files)
{
    if (lang_stats.empty())
    {
        std::cout << "No source files found." << std::endl;
        return;
    }

    if (show_files)
    {
        std::cout << "\n";
        std::cout << "┌─────────────────────────────────────────────────────────────────┐\n";
        std::cout << "│                  File Details                                   │\n";
        std::cout << "├──────────────────────────────────────┬───────┬────────┬─────────┤\n";
        std::cout << "│ File                                 │ Total │ Code   │ Comment │\n";
        std::cout << "├──────────────────────────────────────┼───────┼────────┼─────────┤\n";

        for (const auto& fs : all_files)
        {
            std::string name = fs.filepath;
            if (name.size() > 36) name = "..." + name.substr(name.size() - 33);

            std::printf("│ %-36s │ %5d │ %6d │ %7d │\n", name.c_str(), fs.total_lines, fs.code_lines, fs.comment_lines);
        }
        std::cout << "└──────────────────────────────────────┴───────┴────────┴─────────┘\n";
    }

    std::vector<LangStats> sorted_stats;
    for (const auto& kv : lang_stats)
    {
        sorted_stats.push_back(kv.second);
    }
    std::sort(sorted_stats.begin(), sorted_stats.end(), [](const LangStats& a, const LangStats& b) {
        return a.code_lines > b.code_lines;
    });

    LangStats total;
    total.language = "Total";
    for (const auto& ls : sorted_stats)
    {
        total.files += ls.files;
        total.total_lines += ls.total_lines;
        total.code_lines += ls.code_lines;
        total.comment_lines += ls.comment_lines;
        total.blank_lines += ls.blank_lines;
    }

    std::cout << "\n";
    std::cout << "┌──────────────────────────────┬───────┬───────────┬─────────┬─────────┬───────┐\n";
    std::cout << "│ Language                     │ Files │   Total   │  Code   │ Comment │ Blank │\n";
    std::cout << "├──────────────────────────────┼───────┼───────────┼─────────┼─────────┼───────┤\n";
    for (const auto& ls : sorted_stats)
    {
        std::printf("│ %-28s │ %5d │ %9d │ %7d │ %7d │ %5d │\n",
            ls.language.c_str(), ls.files, ls.total_lines, ls.code_lines, ls.comment_lines, ls.blank_lines);
    }
    std::cout << "├──────────────────────────────┼───────┼───────────┼─────────┼─────────┼───────┤\n";
    std::printf("│ %-28s │ %5d │ %9d │ %7d │ %7d │ %5d │\n",
        total.language.c_str(), total.files, total.total_lines, total.code_lines, total.comment_lines, total.blank_lines);
    std::cout << "└──────────────────────────────┴───────┴───────────┴─────────┴─────────┴───────┘\n";
}
