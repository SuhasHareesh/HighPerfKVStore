#include <WAL.h>

WAL::WAL() {
    std::cout << "WAL File Open for business" << std::endl;
}

WAL::~WAL() {
    std::cout << "WAL File Closed for business" << std::endl;
    fileWriter.close();
}

void WAL::flush() {
    std::lock_guard<std::mutex> lock(fileLock);
    fileWriter.close();

    std::ofstream newWAL(FILENAME, std::ios::trunc);
    newWAL.close();

    fileWriter.open(FILENAME, std::ios::app);
    std::cout << "WAL flushed after snapshot!" << std::endl;
}

void WAL::logPut(const std::string& pKey, const std::string& pValue) {
    std::lock_guard<std::mutex> lock(fileLock);
    fileWriter << "PUT " << pKey << " " << pValue << std::endl;
}

void WAL::logRemove(const std::string &pKey)
{
    std::lock_guard<std::mutex> lock(fileLock);
    fileWriter << "REMOVE " << pKey << std::endl;
}

void WAL::replayWAL(std::unordered_map<std::string, std::string>& pKVStore, LRUCache& pLruList, std::unordered_map<std::string, std::shared_mutex>& pKeyLocks) {

    std::lock_guard<std::mutex> lock(fileLock);
    std::ifstream walReader(FILENAME);

    if(!walReader) {
        std::cerr << "WAL is empty, starting fresh" << std::endl;
        return;
    }

    std::string command;
    std::string key;
    std::string value;

    while(walReader >> command >> key) {
        if(command == "PUT") {
            walReader >> value;
            pKVStore[key] = value;
            pLruList.accessKey(key);
            pKeyLocks.try_emplace(key);
            std::string evictedKey = pLruList.evictKeysIfNeeded();
            if(!evictedKey.empty()) {
                pKVStore.erase(evictedKey);
                pKeyLocks.erase(evictedKey);
            }
        }
        else if(command == "REMOVE") {
            pKVStore.erase(key);
            pLruList.removeKey(key);
        }
    }

    walReader.close();
    std::cout << "WAL replay complete";
    std::cout << "Opening WAL file";
    fileWriter.open(FILENAME, std::ios::app);
}