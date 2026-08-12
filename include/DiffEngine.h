#ifndef DIFF_ENGINE_H
#define DIFF_ENGINE_H

#include <string>
#include <vector>

// Computes a simple line-based diff between two text blobs.
// Uses LCS (Longest Common Subsequence) to identify added and removed lines.
// All methods are stateless; operate on given strings/vectors.
class DiffEngine {
public:
    struct DiffResult {
        bool changed = false;
        int addedLines   = 0;
        int removedLines = 0;
        // Each entry: '+' = added, '-' = removed, ' ' = unchanged
        std::vector<std::pair<char, std::string>> hunks;
    };

    // Compare two text blobs (full file contents as strings).
    // Returns a DiffResult describing the differences.
    static DiffResult compare(const std::string& oldContent,
                               const std::string& newContent);

    // Split text into lines (without trailing newline on each line)
    static std::vector<std::string> splitLines(const std::string& text);
};

#endif
