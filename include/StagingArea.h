#ifndef STAGING_AREA_H
#define STAGING_AREA_H

#include <string>
#include <map>

// Manages the staging area (index): which files are staged for the next commit.
// Persists state to .vcs/staging/index as a simple text format.
// Format: one entry per line — "relative/path hash"
class StagingArea {
private:
    std::string indexPath;    // path to .vcs/staging/index
    std::string objectsDir;   // path to .vcs/objects/

    // In-memory map: relative file path → SHA-256 hash
    std::map<std::string, std::string> index;

    // Parse .vcs/staging/index file into the in-memory map
    void load();

    // Write the in-memory map back to .vcs/staging/index
    void save() const;

    // Store file content as an object in .vcs/objects/<hash>
    // Does nothing if object already exists (content-addressable dedup)
    void storeObject(const std::string& hash, const std::string& content) const;

public:
    StagingArea(const std::string& indexPath, const std::string& objectsDir);

    // Stage one file: hash it, store its content, update index.
    // filePath: path relative to the working directory.
    // fullPath: absolute path to read the file from.
    // Returns true on success.
    bool addFile(const std::string& filePath, const std::string& fullPath);

    // Get the full in-memory index (for use by status, commit later)
    const std::map<std::string, std::string>& getStagedFiles() const;

    // Clear all staged files (called after a commit)
    void clear();
};

#endif
