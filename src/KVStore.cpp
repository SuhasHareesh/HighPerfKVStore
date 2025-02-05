#include<KVStore.h>

KVStore::KVStore() {
    std::cout << "Key-Value Store Initialized" << std::endl;

    bucketStores = std::vector<std::unordered_map<std::string, std::string>>(NUM_BUCKETS);
    bucketMutexes = std::vector<std::mutex>(NUM_BUCKETS);
    

    readFromFile();
}

KVStore::~KVStore() {
    writeToFile();
}

void KVStore::readFromFile() {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "No existing binary database found. Starting fresh." << std::endl;
        return;
    }

    size_t totalSize;
    file.read(reinterpret_cast<char*>(&totalSize), sizeof(size_t));

    for (size_t i = 0; i < totalSize; ++i) {
        size_t keySize, valueSize;
        file.read(reinterpret_cast<char*>(&keySize), sizeof(size_t));

        std::string key(keySize, '\0');
        file.read(&key[0], keySize);
        file.read(reinterpret_cast<char*>(&valueSize), sizeof(size_t));

        std::string value(valueSize, '\0');
        file.read(&value[0], valueSize);

        int bucketIndex = getBucket(key);
        bucketStores[bucketIndex][key] = value;
    }

    file.close();
}

void KVStore::writeToFile() {
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);
    if (!file) {
        std::cerr << "Error: Unable to open file for saving!" << std::endl;
        return;
    }

    size_t totalSize = 0;
    for (const auto& bucket : bucketStores) {
        totalSize += bucket.size();
    }

    file.write(reinterpret_cast<char*>(&totalSize), sizeof(size_t));

    for (int i = 0; i < NUM_BUCKETS; i++) {
        std::lock_guard<std::mutex> lock(bucketMutexes[i]);
        for (const auto& pair : bucketStores[i]) {
            size_t keySize = pair.first.size();
            size_t valueSize = pair.second.size();

            file.write(reinterpret_cast<char*>(&keySize), sizeof(size_t));
            file.write(pair.first.c_str(), keySize);

            file.write(reinterpret_cast<char*>(&valueSize), sizeof(size_t));
            file.write(pair.second.c_str(), valueSize);
        }
    }

    file.close();
}

void KVStore::put(const std::string& pKey, const std::string& pValue) {
    int bucketIndex = getBucket(pKey);
    
    if (bucketIndex < 0 || bucketIndex >= NUM_BUCKETS) {
        std::cerr << "Error: Bucket index out of range for key: " << pKey << std::endl;
        return;
    }

    std::lock_guard<std::mutex> lock(bucketMutexes[bucketIndex]);  // ✅ Safe access
    bucketStores[bucketIndex][pKey] = pValue;
}

std::string KVStore::get(const std::string& pKey) {
    int bucketIdx = getBucket(pKey);
    auto &bucket = bucketStores[bucketIdx];

    auto it = bucket.find(pKey);
    if (it != bucket.end()) {
        return it->second;
    }
    return "Key not found!";
}

void KVStore::remove(const std::string& pKey) {
    int bucket = getBucket(pKey);
    std::lock_guard<std::mutex> lock(bucketMutexes[bucket]);
    bucketStores[bucket].erase(pKey);
}