#include <LRUCache.h>

LRUCache::LRUCache(size_t maxSize) : maxSize(maxSize) {}

void LRUCache::accessKey(const std::string& pKey) {
    std::lock_guard<std::mutex> lock(lruMutex);

    if(keyPosition.find(pKey) != keyPosition.end()) {
        lruList.erase(keyPosition[pKey]);
    }

    lruList.push_front(pKey);
    keyPosition[pKey] = lruList.begin();
}

void LRUCache::removeKey(const std::string& pKey) {
    std::lock_guard<std::mutex> lock(lruMutex);
    if(keyPosition.find(pKey) != keyPosition.end()) {
        lruList.erase(keyPosition[pKey]);
        keyPosition.erase(pKey);
    }
}

void LRUCache::printLRUState() {
    std::lock_guard<std::mutex> lock(lruMutex);
    std::cout << "LRU State: ";
    for (const auto& key : lruList) {
        std::cout << key << " ";
    }
    std::cout << std::endl;
}

void LRUCache::evictKeysIfNeeded(std::unordered_map<std::string, std::string>& pStore) {
    std::lock_guard<std::mutex> lock(lruMutex);
    if(lruList.size() >= maxSize) {
        std::string lruKey = lruList.pop_back();
        keyPosition.erase(lruKey);
        pStore.erase(lruKey);
        
    }
}