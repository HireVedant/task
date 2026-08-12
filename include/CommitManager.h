#ifndef COMMIT_MANAGER_H
#define COMMIT_MANAGER_H

#include "Commit.h"
#include <string>

// Manages commit creation, storage, and retrieval.
// Commits are stored as text files in .vcs/commits/<id>.
// HEAD is stored in .vcs/HEAD as a plain text file containing the current commit ID.
class CommitManager {
private:
    std::string commitsDir;   // path to .vcs/commits/
    std::string headFile;     // path to .vcs/HEAD

    // Serialize a Commit to a text string for disk storage
    std::string serialize(const Commit& c) const;

    // Deserialize a text string back into a Commit
    Commit deserialize(const std::string& text) const;

public:
    CommitManager(const std::string& commitsDir, const std::string& headFile);

    // Get the current HEAD commit ID (empty if no commits yet)
    std::string getHead() const;

    // Set HEAD to a given commit ID
    void setHead(const std::string& commitId);

    // Load a commit from disk by ID. Returns empty Commit if not found.
    Commit getCommit(const std::string& commitId) const;

    // Load the HEAD commit. Returns empty Commit if repository has no commits.
    Commit getHeadCommit() const;

    // Persist a commit to .vcs/commits/<id> and update HEAD.
    void saveCommit(const Commit& c);

    // Build a commit ID: SHA-256 of (message + timestamp + parentId + sorted manifest)
    std::string buildCommitId(const std::string& message,
                               const std::string& timestamp,
                               const std::string& parentId,
                               const std::map<std::string, std::string>& files) const;

    // Check if a commit with the given ID exists
    bool commitExists(const std::string& commitId) const;
};

#endif
