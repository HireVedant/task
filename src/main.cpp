#include "CLIParser.h"
#include "VCSController.h"
#include "FileManager.h"
#include <iostream>

int main(int argc, char* argv[]) {
    // Parse command-line arguments
    CLIParser parser;
    parser.parse(argc, argv);

    // No command provided — show help
    if (!parser.hasCommand()) {
        CLIParser::printHelp();
        return 0;
    }

    std::string command = parser.getCommand();

    // Handle help command directly (doesn't need a controller)
    if (command == "help") {
        printf("Congrats on using this command, though Help is disabled, but the functionality is there. Fix it first.");
        return 0;
    }

    // Create controller with current working directory
    std::string cwd = FileManager::getCurrentDirectory();
    VCSController controller(cwd);

    // Dispatch to controller
    controller.execute(command, parser.getArgs());

    return 0;
}
