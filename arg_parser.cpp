#include "arg_parser.h"

#include <iostream>

// Save one option definition into the parser's registry.
void ArgParser::addOption(const std::string& short_opt, const std::string& long_opt, bool has_value, const std::string& description)
{
    options_.push_back({short_opt, long_opt, has_value, description});
}

// Parse command-line tokens from argv[1..argc-1].
//
// Rules:
// 1) "--name" => long option
// 2) "-n"     => short option
// 3) anything else => positional argument
//
// For options requiring a value, the next argv entry is consumed.
bool ArgParser::parse(int argc, char* argv[])
{
    program_name_ = argv[0];

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        // Handle long options, e.g. "--file".
        if (arg.substr(0, 2) == "--")
        {
            std::string opt_name = arg.substr(2);

            OptionInfo* opt = findByLong(opt_name);
            if (opt == nullptr)
            {
                std::cerr << "Unknown option: " << arg << std::endl;
                return false;
            }
            if (opt->has_value)
            {
                // Ensure there is a following token for the option value.
                if (i + 1 >= argc)
                {
                    std::cerr << "Option requires a value: " << arg << std::endl;
                    return false;
                }
                // Store by long option key for unified access.
                parsed_[opt_name] = argv[++i];
            }
            else
            {
                // For boolean flags, store a textual true marker.
                parsed_[opt_name] = "true";
            }
        }
        // Handle short options, e.g. "-f".
        else if (arg[0] == '-' && arg.size() > 1)
        {
            std::string short_name(1, arg[1]);

            OptionInfo* opt = findByShort(short_name);
            if (!opt)
            {
                std::cerr << "Error: Unknown option: -" << short_name << std::endl;
                return false;
            }
            if (opt->has_value)
            {
                // Ensure there is a following token for the option value.
                if (i + 1 >= argc)
                {
                    std::cerr << "Error: Option requires a value: -" << short_name << std::endl;
                    return false;
                }
                // Normalize key to long option name.
                parsed_[opt->long_opt] = argv[++i];
            }
            else
            {
                // For boolean flags, store a textual true marker.
                parsed_[opt->long_opt] = "true";
            }
        }
        else
        {
            // Any token not recognized as an option is positional.
            positional_args_.push_back(arg);
        }
    }
    return true;
}

// Check whether an option key exists in parsed results.
bool ArgParser::has(const std::string& long_opt) const
{
    return parsed_.count(long_opt) > 0;
}

// Return parsed value or fallback default when option was not provided.
std::string ArgParser::get(const std::string& long_opt, const std::string& default_value) const
{
    auto it = parsed_.find(long_opt);
    if (it != parsed_.end())
    {
        return it->second;
    }
    return default_value;
}

// Expose collected positional arguments.
const std::vector<std::string>& ArgParser::getPositionalArgs() const
{
    return positional_args_;
}

// Render aligned usage text for all registered options.
void ArgParser::printHelp() const
{
    std::cout << "Usage:" << program_name_ << " [options] [args]" << std::endl;
    for (const auto& opt : options_)
    {
        std::string line = "  ";
        if (!opt.short_opt.empty())
        {
            line += "-" + opt.short_opt + ", ";
        }
        else
        {
            line += "    ";
        }
        line += "--" + opt.long_opt;

        // Pad to a fixed width so descriptions line up vertically.
        while (line.length() < 25)
        {
            line += " ";
        }
        line += opt.description;
        std::cout << line << "\n";
    }
}

// Linear search in option definitions by short key.
ArgParser::OptionInfo* ArgParser::findByShort(const std::string& s)
{
    for (auto& opt : options_)
    {
        if (opt.short_opt == s)
        {
            return &opt;
        }
    }
    return nullptr;
}

// Linear search in option definitions by long key.
ArgParser::OptionInfo* ArgParser::findByLong(const std::string& l)
{
    for (auto& opt : options_)
    {
        if (opt.long_opt == l)
        {
            return &opt;
        }
    }
    return nullptr;
}
