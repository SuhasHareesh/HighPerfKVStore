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
// For Debuggin purposes
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
        keyPosition.erase(lruList.back());
        pStore.erase(lruList.back());
        lruList.pop_back();
    }
}

void LRUCache::saveState() {
    std::lock_guard<std::mutex> lock(lruMutex);
    std::ofstream file(FILENAME, std::ios::binary | std::ios::trunc);

    size_t lruSize = lruList.size();
    file.write(reinterpret_cast<const char*>(&lruSize), sizeof(size_t));

    for (const auto& key: lruList) {
        size_t keySize = key.size();
        file.write(reinterpret_cast<const char*>(&keySize), sizeof(size_t));
        file.write(key.c_str(), keySize);
    }

    file.close();
}

void LRUCache::loadState() {
    std::lock_guard<std::mutex> lock(lruMutex);
    std::ifstream file(FILENAME, std::ios::binary);

    lruList.clear();
    keyPosition.clear();

    size_t lruSize;
    file.read(reinterpret_cast<char*>(&lruSize), sizeof(size_t));
    for (size_t i = 0; i < lruSize; i++) {
        size_t keySize;
        file.read(reinterpret_cast<char*>(&keySize), sizeof(size_t));

        std::string key(keySize, '\0');
        file.read(&key[0], keySize);

        lruList.push_back(key);
        keyPosition[key] = std::prev(lruList.end());
    }

    file.close();
}