#include "DiffEngine.h"
#include <sstream>
#include <algorithm>

// Split a string into lines. Handles \n and \r\n line endings.
std::vector<std::string> DiffEngine::splitLines(const std::string& text) {
    std::vector<std::string> lines;
    std::istringstream stream(text);
    std::string line;
    while (std::getline(stream, line)) {
        // Strip trailing \r for CRLF files
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }
    return lines;
}

// LCS-based diff using the Myers/DP approach (O(mn) space).
// Builds a standard DP table and backtracks to produce the edit script.
DiffEngine::DiffResult DiffEngine::compare(const std::string& oldContent,
                                            const std::string& newContent) {
    DiffResult result;

    std::vector<std::string> oldLines = splitLines(oldContent);
    std::vector<std::string> newLines = splitLines(newContent);

    const int m = static_cast<int>(oldLines.size());
    const int n = static_cast<int>(newLines.size());

    // Build LCS DP table
    // dp[i][j] = length of LCS of oldLines[0..i-1] and newLines[0..j-1]
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (oldLines[i - 1] == newLines[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    // Backtrack to build the diff hunks
    std::vector<std::pair<char, std::string>> hunks;
    int i = m, j = n;
    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && oldLines[i - 1] == newLines[j - 1]) {
            hunks.push_back({' ', oldLines[i - 1]});
            --i; --j;
        } else if (j > 0 && (i == 0 || dp[i][j - 1] >= dp[i - 1][j])) {
            hunks.push_back({'+', newLines[j - 1]});
            ++result.addedLines;
            --j;
        } else {
            hunks.push_back({'-', oldLines[i - 1]});
            ++result.removedLines;
            --i;
        }
    }

    // Hunks were built in reverse; correct the order
    std::reverse(hunks.begin(), hunks.end());

    result.hunks  = std::move(hunks);
    result.changed = (result.addedLines > 0 || result.removedLines > 0);
    return result;
}
