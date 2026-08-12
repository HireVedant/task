#ifndef VCS_CONTROLLER_H
#define VCS_CONTROLLER_H

#include <string>
#include <vector>
#include "RepositoryManager.h"

// Central orchestrator: receives parsed commands and delegates to the right module.
// Each command has its own handler method to keep the logic readable.
class VCSController {
private:
    std::string workingDir;

public:
    explicit VCSController(const std::string& workingDir);

    // Dispatch a command with its arguments to the correct handler
    void execute(const std::string& command, const std::vector<std::string>& args);

private:
    // Core command handlers
    void handleInit();
    void handleAdd(const std::vector<std::string>& args);
    void handleStatus();
    void handleCommit(const std::vector<std::string>& args);
    void handleLog();
    void handleCheckout(const std::vector<std::string>& args);
    void handleDiff(const std::vector<std::string>& args);

    // Branch-related handlers
    void handleBranch(const std::vector<std::string>& args);
    void handleMerge(const std::vector<std::string>& args);

    // History handlers
    void handleRevert(const std::vector<std::string>& args);
    void handleGraph();

    // Informational
    void handleStats();

    // Python analytics integration
    void handleAnalyze(const std::vector<std::string>& args);
    void handleReport();

    // Helper: prints error + returns false if repo is not initialized
    bool requireRepo();

    // Helper: collect repository data for Python analytics
    void collectRepoData();
};

#endif
