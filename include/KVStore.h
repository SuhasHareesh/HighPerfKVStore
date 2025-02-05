#ifndef KVSTORE_H
#define KVSTORE_H

#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <mutex>
#include <atomic>
#include <memory> 

class KVStore {
    private:
        static const int NUM_BUCKETS = 16;
        std::vector<std::unordered_map<std::string, std::string>> bucketStores;
        std::vector<std::mutex> bucketMutexes;

        const std::string filename = "kvstore.kv";

        void writeToFile();
        void readFromFile();

        int getBucket(const std::string &pKey) const {
            return std::hash<std::string>{}(pKey) % NUM_BUCKETS;
        }

    public:
        KVStore();
        ~KVStore();
        void put(const std::string& pKey, const std::string& pValue);
        std::string get(const std::string& pKey);
        void remove(const std::string& pKey);
};

#endif