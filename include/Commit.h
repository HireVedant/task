#ifndef COMMIT_H
#define COMMIT_H

#include <string>
#include <map>

// Plain data model representing one commit snapshot.
// Stored as a text file in .vcs/commits/<id>
struct Commit {
    std::string id;         // SHA-256 of content
    std::string message;
    std::string timestamp;
    std::string parentId;   // empty string for the root commit

    // Complete file manifest: relative path → SHA-256 of content
    std::map<std::string, std::string> files;
};

#endif
