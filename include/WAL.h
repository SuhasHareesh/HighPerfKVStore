#ifndef WAL_H
#define WAL_H

#include <fstream>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <iostream>
#include <unordered_map>

#include <LRUCache.h>

class WAL {
    private:
        const std::string FILENAME = "KVStore.wal";

        std::ofstream fileWriter;
        std::mutex fileLock;

    public:
    // Constructors & Destructors
        WAL();
        ~WAL();

    // Member Functions
        void    logPut      (const std::string&, const std::string&);
        void    logRemove   (const std::string&);
        void    flush       ();
        void    replayWAL   (std::unordered_map<std::string, std::string>&, LRUCache&, std::unordered_map<std::string, std::shared_mutex>&);
};


#endif
