#ifndef KVSTORE_H
#define KVSTORE_H

#include<unordered_map>
#include<string>
#include<iostream>
#include<fstream>

class KVStore {
    private:
        std::unordered_map<std::string, std::string> store;
        const std::string filename = "kvstore.kv";
        void writeToFile();
        void readFromFile();

    public:
        KVStore();
        ~KVStore();
        void put(const std::string& pKey, const std::string& pValue);
        std::string get(const std::string& pKey);
        void remove(const std::string& pKey);
};

#endif