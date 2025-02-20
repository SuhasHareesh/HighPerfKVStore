#ifndef KVSTORE_H
#define KVSTORE_H

#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <shared_mutex>
#include <thread>

#include <WAL.h>
#include <Snapshot.h>
#include <LRUCache.h>


class KVStore {
    private:
        Snapshot snapshot;
        WAL writeAheadLogger;
        LRUCache lruCache = LRUCache(10);
    
        std::unordered_map<std::string, std::string> store;
        std::unordered_map<std::string, std::shared_mutex> keyLocks;
        mutable std::mutex keyLocksMutex;

    public:
        KVStore();
        ~KVStore();

        void put(const std::string&, const std::string&);
        std::string get(const std::string&);
        void remove(const std::string&);
};

#endif