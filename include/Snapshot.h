#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <fstream>
#include <iostream>
#include <shared_mutex>
#include <unordered_map>
#include <string>

#include <LRUCache.h>

class Snapshot {
    private:
        const std::string FILENAME = "KVStore.snapshot";
        mutable std::shared_mutex snapshotLock;
    
    public:

        void saveSnapshot(const std::unordered_map<std::string, std::string>&, const LRUCache&);
        void loadSnapshot(std::unordered_map<std::string, std::string>&, LRUCache&);

};

#endif