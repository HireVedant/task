#include "StagingArea.h"
#include "FileManager.h"
#include <iostream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

// --- Persistence format (.vcs/staging/index) ---
// Simple text format, one entry per line: "relative/path<TAB>sha256hash"
// An empty file means no staged files.
// This is easier to read/write than JSON while still being human-readable.

StagingArea::StagingArea(const std::string& idxPath, const std::string& objsDir)
    : indexPath(idxPath), objectsDir(objsDir) {
    load();
}

void StagingArea::load() {
    index.clear();
    std::string content = FileManager::readFile(indexPath);
    if (content.empty()) return;

    std::istringstream stream(content);
    std::string line;
    while (std::getline(stream, line)) {
        if (line.empty()) continue;
        // Each line: "path\thash"
        size_t tabPos = line.find('\t');
        if (tabPos == std::string::npos) continue;
        std::string path = line.substr(0, tabPos);
        std::string hash = line.substr(tabPos + 1);
        if (!path.empty() && !hash.empty()) {
            index[path] = hash;
        }
    }
}

void StagingArea::save() const {
    std::ostringstream oss;
    for (const auto& [path, hash] : index) {
        oss << path << '\t' << hash << '\n';
    }
    FileManager::writeFile(indexPath, oss.str());
}

void StagingArea::storeObject(const std::string& hash, const std::string& content) const {
    std::string objectPath = objectsDir + "/" + hash;
    // Content-addressable deduplication: skip if already stored
    if (FileManager::exists(objectPath)) {
        return;
    }
    FileManager::writeFile(objectPath, content);
}

bool StagingArea::addFile(const std::string& filePath, const std::string& fullPath) {
    // Read file content
    std::string content = FileManager::readFile(fullPath);
    if (content.empty() && !FileManager::exists(fullPath)) {
        std::cerr << "Error: Could not read file '" << filePath << "'." << std::endl;
        return false;
    }

    // Compute SHA-256 hash
    std::string hash = FileManager::computeHash(content);
    if (hash.empty()) {
        std::cerr << "Error: Hashing failed for '" << filePath << "'." << std::endl;
        return false;
    }

    // Store content in .vcs/objects/ (deduplication handled inside)
    storeObject(hash, content);

    // Update in-memory index (overwrites if file was re-staged)
    index[filePath] = hash;

    // Persist to disk
    save();
    return true;
}

const std::map<std::string, std::string>& StagingArea::getStagedFiles() const {
    return index;
}

void StagingArea::clear() {
    index.clear();
    save();
}
