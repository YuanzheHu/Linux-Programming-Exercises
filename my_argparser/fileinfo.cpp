#include <iostream>
#include <string>
#include <unistd.h>

#include "arg_parser.h"
#include "file_info.h"

// Entry point: configures options, validates input, then dispatches output.
int main(int argc, char* argv[])
{
    // 1) Register all supported CLI options.
    ArgParser parser;
    parser.addOption("h", "help", false, "Show this help message");
    parser.addOption("V", "version", false, "Show program version");
    parser.addOption("f", "file", true, "File path to show information");
    parser.addOption("v", "verbose", false, "Show detailed information");
    parser.addOption("", "format", true, "Output format (text/json)");

    // 2) Parse argv. On parse errors, print help and exit with non-zero code.
    if (!parser.parse(argc, argv))
    {
        parser.printHelp();
        return 1;
    }

    // 3) Show help when requested or when no arguments are provided.
    if (parser.has("help") || argc == 1)
    {
        parser.printHelp();
        return 0;
    }

    // 4) Version command is independent and exits immediately.
    if (parser.has("version"))
    {
        std::cout << "FileInfo version 1.0" << std::endl;
        std::cout<<"Author: Yuanzhe Hu" << std::endl;
        std::cout <<"PID: " << getpid() << std::endl;
        return 0;
    }

    // 5) Main command path: read target file + optional flags.
    if (parser.has("file"))
    {
        std::string filepath = parser.get("file");
        bool verbose = parser.has("verbose");
        std::string format = parser.get("format", "normal");

        // Guard against unsupported output format values.
        if (format != "normal" && format != "json")
        {
            std::cerr << "Error: Invalid format. Use 'normal' or 'json'." << std::endl;
            return 1;
        }

        // Display collected metadata.
        showFileInfo(filepath, verbose, format);
        return 0;
    }

    // 6) Fallback error when no file option is provided.
    std::cerr << "Error: No file specified. Use -f or --file option." << std::endl;
    std::cerr << "Use -h or --help for usage information." << std::endl;
    return 1;
}