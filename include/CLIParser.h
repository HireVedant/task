#ifndef CLI_PARSER_H
#define CLI_PARSER_H

#include <string>
#include <vector>

// Parses command-line arguments into a command name and its arguments.
class CLIParser {
private:
    std::string command;
    std::vector<std::string> args;

public:
    CLIParser();

    // Parse argc/argv into command + arguments
    void parse(int argc, char* argv[]);

    // Getters
    std::string getCommand() const;
    std::vector<std::string> getArgs() const;
    bool hasCommand() const;

    // Display help menu
    static void printHelp();
};

#endif
