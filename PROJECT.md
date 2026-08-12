# Mini VCS (Version Control System)

A fully functional CLI Version Control System built as a **hybrid C++ + Python** application, inspired by Git.

---

## 🏗 C++ + Python Architecture

```
            ┌──────────────────────┐
            │      CLI / User      │
            └──────────┬───────────┘
                       │
                       ▼
            ┌──────────────────────┐
            │    C++ Mini VCS      │  ← Core VCS engine
            │      Core Engine     │     (hashing, commits, branches,
            └──────────┬───────────┘      merge, diff, checkout)
                       │
           Repository data → JSON
                       │
                       ▼
            ┌──────────────────────┐
            │    Python Analytics  │  ← Analytics layer
            │       Layer          │     (stats, health, reports)
            └──────────┬───────────┘
                       │
          ┌────────────┼─────────────┐
          ▼            ▼             ▼
      Statistics    Reports      Visualization
```

**Why two languages?**

- **C++** handles the core VCS because it requires low-level filesystem control, SHA-256 hashing (OpenSSL), and OOP architecture for commit snapshots, branching, and merge algorithms.
- **Python** is integrated as an analytics and reporting layer because Python provides simpler data processing, statistical analysis, and HTML report generation using its standard library.
- The two layers communicate through **structured JSON** and a controlled **subprocess interface** (`PythonBridge` class).

---

## ✨ Features

### Core Operations (C++)
| Command | Description |
|---|---|
| `vcs init` | Initialize a repository |
| `vcs add <file> [file...]` | Stage files for commit |
| `vcs status` | Show file states (Staged/Modified/Untracked/Unchanged/Deleted) |
| `vcs commit "<message>"` | Create a commit snapshot |
| `vcs log` | Display commit history |
| `vcs checkout <commit\|branch>` | Restore a commit or switch branch |
| `vcs diff <file>` | Show line-level changes against HEAD |

### Branching & Advanced (C++)
| Command | Description |
|---|---|
| `vcs branch` | List all branches |
| `vcs branch <name>` | Create a new branch at HEAD |
| `vcs merge <branch>` | 3-way merge with conflict detection |
| `vcs revert <commit>` | Undo a commit (creates new commit) |
| `vcs graph` | ASCII commit graph with branch labels |
| `vcs stats` | Show repository statistics |

### Analytics & Reports (Python)
| Command | Description |
|---|---|
| `vcs analyze` | Full repository analytics summary |
| `vcs analyze --health` | Repository health score (0–100) |
| `vcs analyze --json` | Export analytics as JSON |
| `vcs analyze --csv` | Export analytics as CSV |
| `vcs report` | Generate HTML analytics report |

### Other
- `.vcsignore` support (exact paths, `*.ext` wildcards, `dir/` patterns)
- `vcs help` — full command reference

---

## 📂 Project Structure

```
├── Makefile                    # Build system
├── include/                    # C++ headers
│   ├── BranchManager.h
│   ├── CLIParser.h
│   ├── Commit.h
│   ├── CommitManager.h
│   ├── DiffEngine.h
│   ├── FileManager.h
│   ├── PythonBridge.h          # C++ ↔ Python interface
│   ├── RepositoryManager.h
│   ├── StagingArea.h
│   └── VCSController.h
├── src/                        # C++ implementations
│   ├── BranchManager.cpp
│   ├── CLIParser.cpp
│   ├── CommitManager.cpp
│   ├── DiffEngine.cpp
│   ├── FileManager.cpp
│   ├── PythonBridge.cpp        # JSON serializer + subprocess invoker
│   ├── RepositoryManager.cpp
│   ├── StagingArea.cpp
│   ├── VCSController.cpp
│   └── main.cpp
├── python/                     # Python analytics layer
│   ├── analytics.py            # CLI entry point
│   ├── repository_analyzer.py  # Stats, health score, commit analysis
│   ├── report_generator.py     # HTML report generation
│   └── utils.py                # Shared helpers
```

---

## 🧬 Class Responsibilities

| Class | Language | Purpose |
|---|---|---|
| `CLIParser` | C++ | Parse `argv`, display help menu |
| `VCSController` | C++ | Command dispatcher → delegates to modules |
| `RepositoryManager` | C++ | `.vcs/` directory structure and path management |
| `FileManager` | C++ | File I/O, SHA-256 hashing (OpenSSL EVP), `.vcsignore` matching |
| `StagingArea` | C++ | Staging index read/write, object storage |
| `CommitManager` | C++ | Commit serialization, HEAD management, parent chain |
| `BranchManager` | C++ | Branch create/list/switch, current branch tracking |
| `DiffEngine` | C++ | LCS-based line diff algorithm |
| `PythonBridge` | C++ | Serialize repo data → JSON, invoke Python subprocess |
| `RepositoryAnalyzer` | Python | Commit stats, file frequency, branch analysis, health score |
| `ReportGenerator` | Python | HTML report with inline CSS |

---

## 🔄 C++ ↔ Python Communication Flow

```
1. User runs:       ./vcs analyze --health
2. C++ dispatch:    VCSController::handleAnalyze(["--health"])
3. C++ collects:    commits, branches, files, objects, sizes, staging state
4. PythonBridge:    serializes RepoData → JSON → writes .vcs/analytics_input.json
5. PythonBridge:    invokes:  python3 python/analytics.py .vcs/analytics_input.json --health
6. Python reads:    JSON input (does NOT access .vcs/ internals)
7. Python outputs:  formatted health report to stdout
8. PythonBridge:    cleans up temp JSON file
```

---

## 🛠 Build & Run

**Prerequisites:**
- `g++` (C++17 support)
- OpenSSL dev libraries (`-lcrypto`)
- Python 3 (for analytics commands only — core VCS works without it)

```bash
# Build
make clean && make

# Basic workflow
./vcs init
echo "Hello" > hello.cpp
./vcs add hello.cpp
./vcs commit "First commit"
./vcs log

# Branching
./vcs branch feature
./vcs checkout feature
echo "Feature code" > feature.cpp
./vcs add feature.cpp
./vcs commit "Add feature"
./vcs checkout main
./vcs merge feature

# Analytics (requires Python 3)
./vcs analyze
./vcs analyze --health
./vcs analyze --json
./vcs report
```

---

## 📊 Repository Structure (`.vcs/`)

```
.vcs/
├── HEAD                    # Current commit ID
├── BRANCH                  # Current branch name
├── staging/
│   └── index               # Staged files: "path\thash" per line
├── commits/
│   └── <sha256id>          # Serialized commit files
├── branches/
│   └── <branch-name>       # Branch → commit ID mapping
├── objects/
│   └── <sha256hash>        # Content-addressable file objects
└── reports/
    └── repository_report.html  # Generated HTML report
```

---

## 🧪 Testing

**51 tests**, all passing:
- Core commands: init, add, status, commit, log, diff, checkout
- Branching: create, list, switch, duplicate detection
- Merge: non-conflicting + conflict detection with markers
- Revert: snapshot-based inverse commit
- Graph, Stats, .vcsignore, persistence
- Python: analyze, --health, --json, --csv, report generation
- Error cases: non-repo, no commits, invalid IDs, missing files

---

## ⚠️ Known Limitations

| Feature | Limitation |
|---|---|
| Merge | Snapshot-based 3-way; no recursive LCA for diamond merges |
| Revert | Snapshot inversion (not patch-level inverse diff) |
| Graph | Shows HEAD's parent chain only; diverged branches annotated but not drawn side-by-side |
| Detached HEAD | Commits in detached state don't advance any branch |
| Python | Required only for analytics; core VCS works without Python installed |
| Health score | Transparent but simple heuristic (no ML) |
