#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <map>
#include <string>
#include <vector>

// Lightweight command-line argument parser for short and long options.
//
// Example supported forms:
//   -h
//   -f path.txt
//   --help
//   --file path.txt
//
// Parsed values are stored by long option name, so both "-f" and
// "--file" can be queried via has("file") / get("file").
class ArgParser
{
public:
    // Registers one option definition.
    // short_opt: one-character flag without leading '-' (e.g. "h").
    // long_opt : long flag without leading '--' (e.g. "help").
    // has_value: true when this option requires a following value.
    // description: text shown in printHelp().
    void addOption(const std::string& short_opt, const std::string& long_opt, bool has_value, const std::string& description);

    // Parses argv and fills internal maps/vectors.
    // Returns false when it encounters an unknown option or missing value.
    bool parse(int argc, char* argv[]);

    // Returns true if an option was provided by the user.
    bool has(const std::string& long_opt) const;

    // Returns the parsed value for an option or default_value if absent.
    std::string get(const std::string& long_opt, const std::string& default_value = "") const;

    // Returns non-option arguments, in original order.
    const std::vector<std::string>& getPositionalArgs() const;

    // Prints usage and all registered options.
    void printHelp() const;

private:
    // Internal metadata for each supported option.
    struct OptionInfo
    {
        std::string short_opt;
        std::string long_opt;
        bool has_value;
        std::string description;
    };

    // Lookup helpers used during parse().
    OptionInfo* findByShort(const std::string& s);
    OptionInfo* findByLong(const std::string& l);

    // Option registry built via addOption().
    std::vector<OptionInfo> options_;

    // Parsed option values keyed by long option name.
    std::map<std::string, std::string> parsed_;

    // Stores positional (non-option) arguments.
    std::vector<std::string> positional_args_;

    // Program executable name from argv[0], used in help output.
    std::string program_name_;
};

#endif
