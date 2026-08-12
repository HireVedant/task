#ifndef PYTHON_BRIDGE_H
#define PYTHON_BRIDGE_H

#include <string>
#include <vector>
#include <map>
#include "Commit.h"

// Handles communication between C++ VCS engine and Python analytics layer.
//
// Flow:
//   1. C++ collects repository data (commits, branches, files, etc.)
//   2. PythonBridge serializes the data into JSON
//   3. PythonBridge writes JSON to a temp file
//   4. PythonBridge invokes: python3 python/analytics.py <tempfile> [--flags]
//   5. Python reads the JSON, performs analytics, and prints results
//   6. PythonBridge cleans up the temp file
//
// This keeps Python completely read-only — it never touches .vcs/ internals.
class PythonBridge {
public:
    // Repository snapshot data to pass to Python
    struct RepoData {
        std::string activeBranch;
        std::vector<std::string> branches;
        int totalObjects    = 0;
        double repoSizeKB   = 0.0;
        std::vector<Commit> commits;           // full commit chain (newest first)
        std::map<std::string, std::string> headFiles;  // current HEAD file manifest
        int stagedCount     = 0;
        int modifiedCount   = 0;
        int untrackedCount  = 0;
    };

    // Locate a working Python 3 executable. Returns "" if not found.
    static std::string findPython();

    // Serialize RepoData to a JSON string
    static std::string toJson(const RepoData& data);

    // Invoke the Python analytics script with the given flags.
    // pythonDir = path to the python/ directory (relative to repo root)
    // Returns the exit code from the Python process.
    static int invoke(const std::string& repoRoot,
                      const RepoData& data,
                      const std::vector<std::string>& flags);
};

#endif
