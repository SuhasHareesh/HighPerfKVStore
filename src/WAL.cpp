#include <WAL.h>

WAL::WAL() {
    std::cout << "WAL File Open for business" << std::endl;
    fileWriter.open(WALFILE, std::ios::app);
}

WAL::~WAL() {
    std::cout << "WAL File Closed for business" << std::endl;
    fileWriter.close();
}

void WAL::flush() {
    std::lock_guard<std::mutex> lock(fileLock);
    fileWriter.close();

    std::ofstream newWAL(WALFILE, std::ios::trunc);
    newWAL.close();

    walFile.open(WALFILE, std::ios::app);
    std::cout << "WAL flushed after snapshot!" << std::endl;
}

void WAL::logPut(const std::string& pKey, const std::string& pValue) {
    std::lock_guard<std::mutex> lock(fileLock);
    walFile << "PUT " << pKey << " " << pValue << std::endl;
}

void WAL::logRemove(const std::string &pKey)
{
    std::lock_guard<std::mutex> lock(fileLock);
    walFile << "REMOVE " << pKey << std::endl;
}

void WAL::replayWAL(std::unordered_map<std::string, std::string>& pKVStore, LRUCache& lruList) {

    std::lock_guard<std::mutex> lock(fileLock);
    std::ifstream walReader(WALFILE);

    if(!walReader) {
        std::cerr << "Error opening WAL file" << std::endl;
        return;
    }

    std::string command;
    std::string key;
    std::string value;

    while(walReader >> command >> key) {
        if(command == "PUT") {
            walReader >> value;
            pKVStore[key] = value;
            lruList.accessKey(key);
            lruList.evictKeysIfNeeded(store);
        }
        else if(command == "REMOVE") {
            pKVStore.erase(key);
            lruList.removeKey(key);
        }
    }

    walReader.close();
    std::cout << "WAL replay complete";
}