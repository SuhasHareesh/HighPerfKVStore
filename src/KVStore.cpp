#include<KVStore.h>

KVStore::KVStore() {
    std::cout << "Key-Value Store Initialized" << std::endl;
    readFromFile();
}

KVStore::~KVStore() {
    writeToFile();
}

void KVStore::readFromFile() {
    std::ifstream file(filename);
    if(!file) {
        std::cerr << "No existing database found. Starting fresh." << std::endl;
        return;
    }
    std::string key, val;
    while (file >> key >> val) {
        store[key] = val;
    }
    file.close();
    return;
}

void KVStore::writeToFile() {
    std::ofstream file(filename);
    if(!file) {
        std::cerr << "Unable to open file for saving\n";
        return;
    }

    for (const auto& pair : store) {
        file << pair.first << " " << pair.second << "\n";
    }
    file.close();
    return;
}

void KVStore::put(const std::string& pKey, const std::string& pValue) {
    store[pKey] = pValue;
}

std::string KVStore::get(const std::string& key) {
    if (store.find(key) != store.end()) {
        return store[key];
    }
    return "Key not found!";
}

void KVStore::remove(const std::string& pKey) {
    store.erase(pKey);
}