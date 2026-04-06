#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "cli.h"
#include "file_analyzer.h"
#include "file_collector.h"
#include "report.h"
#include "stats.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printHelp(argv[0]);
        return 1;
    }

    std::string target_path;
    std::vector<std::string> filter_exts;
    bool show_files = false;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h")
        {
            printHelp(argv[0]);
            return 0;
        }
        else if (arg == "--ext" && i + 1 < argc)
        {
            filter_exts = splitExtensions(argv[++i]);
        }
        else if (arg == "--show-files")
        {
            show_files = true;
        }
        else if (arg[0] != '-')
        {
            target_path = arg;
        }
        else
        {
            std::cerr << "Unknown option: " << arg << std::endl;
            printHelp(argv[0]);
            return 1;
        }
    }

    if (target_path.empty())
    {
        std::cerr << "Error: No target path specified.\n";
        printHelp(argv[0]);
        return 1;
    }

    std::vector<FileStats> all_files_stats;
    std::map<std::string, LangStats> lang_stats;
    std::vector<std::string> out_files;

    collectFiles(target_path, filter_exts, out_files);

    for (const std::string& filepath : out_files)
    {
        FileStats fs;
        if (analyzeFile(filepath, fs))
        {
            all_files_stats.push_back(fs);
            lang_stats[fs.language].language = fs.language;
            lang_stats[fs.language].merge(fs);
        }
    }

    printReport(lang_stats, all_files_stats, show_files);

    return 0;
}
