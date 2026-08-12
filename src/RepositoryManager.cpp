#include "RepositoryManager.h"
#include "FileManager.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

RepositoryManager::RepositoryManager(const std::string& rootPath) : repoRoot(rootPath) {
    buildPaths();
}

void RepositoryManager::buildPaths() {
    vcsDir      = repoRoot + "/.vcs";
    stagingDir  = vcsDir + "/staging";
    commitsDir  = vcsDir + "/commits";
    objectsDir  = vcsDir + "/objects";
    branchesDir = vcsDir + "/branches";
    headFile    = vcsDir + "/HEAD";
    branchFile  = vcsDir + "/BRANCH";
    indexFile   = stagingDir + "/index";
}

bool RepositoryManager::initRepository() {
    if (isInitialized()) {
        std::cout << "Error: Repository already initialized." << std::endl;
        return false;
    }

    // Create all required subdirectories
    bool ok = FileManager::createDirectory(vcsDir)
           && FileManager::createDirectory(stagingDir)
           && FileManager::createDirectory(commitsDir)
           && FileManager::createDirectory(objectsDir)
           && FileManager::createDirectory(branchesDir);

    if (!ok) {
        std::cerr << "Error: Failed to create repository structure." << std::endl;
        return false;
    }

    // HEAD starts empty (no commits yet)
    FileManager::writeFile(headFile, "");

    // Default branch is "main"
    FileManager::writeFile(branchFile, "main\n");

    // Create the "main" branch file with no commit yet
    FileManager::writeFile(branchesDir + "/main", "");

    // Empty staging index
    FileManager::writeFile(indexFile, "");

    std::cout << "Repository initialized successfully." << std::endl;
    return true;
}

bool RepositoryManager::isInitialized() const {
    return FileManager::exists(vcsDir);
}

std::string RepositoryManager::getRepoRoot()    const { return repoRoot; }
std::string RepositoryManager::getVcsDir()      const { return vcsDir; }
std::string RepositoryManager::getStagingDir()  const { return stagingDir; }
std::string RepositoryManager::getCommitsDir()  const { return commitsDir; }
std::string RepositoryManager::getObjectsDir()  const { return objectsDir; }
std::string RepositoryManager::getBranchesDir() const { return branchesDir; }
std::string RepositoryManager::getHeadFile()    const { return headFile; }
std::string RepositoryManager::getBranchFile()  const { return branchFile; }
std::string RepositoryManager::getIndexFile()   const { return indexFile; }
