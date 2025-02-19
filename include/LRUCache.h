#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <list>
#include <mutex>

class LRUCache {
    private:
        size_t maxSize;
        std::list<std::string> lruList;
        std::unordered_map<std::string, std::list<std::string>::iterator> keyPosition;
        std::mutex lruMutex;

    public:
        explicit LRUCache(size_t maxSize);
        void accessKey(const std::string&);
        void removeKey(const std::string&);
        void evictKeysIfNeeded(std::unordered_map<std::string, std::string>&);
        void printLRUState();     
}

#endif