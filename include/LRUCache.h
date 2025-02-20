#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <list>
#include <mutex>
#include <fstream>

class LRUCache {
    private:
        const std::string FILENAME = "KVStore.lru";

        size_t maxSize;
        std::list<std::string> lruList;
        std::unordered_map<std::string, std::list<std::string>::iterator> keyPosition;
        std::mutex lruMutex;

    public:
        explicit LRUCache(size_t);
        void accessKey(const std::string&);
        void removeKey(const std::string&);
        std::string evictKeysIfNeeded();
        void printLRUState();
        
        void saveState();
        void loadState();
};

#endif