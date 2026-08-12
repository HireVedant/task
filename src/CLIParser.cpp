#include "CLIParser.h"
#include <iostream>

CLIParser::CLIParser() : command(""), args() {}

void CLIParser::parse(int argc, char* argv[]) {
    // argv[0] = binary name ("vcs"), argv[1] = command, argv[2+] = arguments
    if (argc < 2) { command = ""; return; }
    command = argv[1];
    for (int i = 2; i < argc; ++i)
        args.push_back(argv[i]);
}

std::string CLIParser::getCommand() const { return command; }
std::vector<std::string> CLIParser::getArgs() const { return args; }
bool CLIParser::hasCommand() const { return !command.empty(); }

void CLIParser::printHelp() {
    std::cout <<
        "Mini VCS — A simple version control system\n"
        "\n"
        "Usage: vcs <command> [arguments]\n"
        "\n"
        "Core:\n"
        "  init                       Initialize a new repository\n"
        "  add <file> [file...]       Stage files for commit\n"
        "  status                     Show file states\n"
        "  commit \"<message>\"         Create a commit snapshot\n"
        "  log                        Display commit history\n"
        "  checkout <commit|branch>   Restore a commit or switch branch\n"
        "  diff <file>                Show changes against HEAD\n"
        "\n"
        "Branching:\n"
        "  branch                     List all branches\n"
        "  branch <name>              Create a new branch at HEAD\n"
        "  merge <branch>             Merge a branch into the current branch\n"
        "\n"
        "History:\n"
        "  revert <commit>            Undo a past commit (creates new commit)\n"
        "  graph                      Show a text-based commit graph\n"
        "\n"
        "Analytics (Python):\n"
        "  analyze                    Show repository analytics\n"
        "  analyze --health           Show repository health score\n"
        "  analyze --json             Export analytics as JSON\n"
        "  analyze --csv              Export analytics as CSV\n"
        "  report                     Generate HTML analytics report\n"
        "\n"
        "Other:\n"
        "  stats                      Show repository statistics\n"
        "  help                       Show this help message\n";
}
