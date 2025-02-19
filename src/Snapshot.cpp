#include <Snapshot.h>

void Snapshot::saveSnapshot(const std::unordered_map<std::string, std::string>& pKVStore, const LRUCache& pLruList) {
    std::lock_guard<std::mutex> lock(snapshotLock);
    std::ofstream snapshotWriter(FILENAME, std::ios::binary | std::ios::trunc);
    if (!snapshotWriter) {
        std::cerr << "Error: Unable to open snapshot." << std::endl;
        return;
    }

    size_t numEntries = pKVStore.size();
    snapshotWriter.write(reinterpret_cast<const char*>(&numEntries), sizeof(size_t));

    for(const auto& pair : pKVStore) {
        size_t keySize = pair.first.size();
        size_t valueSize = pair.second.first.size();

        snapshotWriter.write(reinterpret_cast<const char*>(&keySize), sizeof(keySize));
        snapshotWriter.write(pair.first.c_str(), keySize);
        snapshotWriter.write(reinterpret_cast<const char*>(&valueSize), sizeof(valueSize));
        snapshotWriter.write(pair.second.c_str(), valueSize);
    }

    pLruList.saveState();
    snapshotWriter.close();
    std::cout << "Snapshot saved successfully" << std::endl;
}

void Snapshot::loadSnapshot(std::unordered_map<std::string, std::string>& pKVStore, LRUCache& pLruList) {
    std::lock_guard<std::mutex> lock(snapshotLock);
    std::ifstream snapshotReader(FILENAME, std::ios::binary);
    if (!snapshotReader) {
        std::cerr << "No snapshot found, starting fresh." << std::endl;
        return;
    }

    size_t numEntries;
    snapshotReader.read(reinterpret_cast<char *>(&numEntries), sizeof(size_t));

    if (!snapshotReader) {
        std::cerr << "Snapshot file not found, starting fresh." << std::endl;
        return;
    }

    for(size_t i = 0; i < numEntries; i++) {
        size_t keySize, valueSize;

        snapshotReader.read(reinterpret_cast<char *>(&keySize), sizeof(size_t));
        std::string key(keySize, '\0');
        snapshotReader.read(&key[0], keySize);

        snapshotReader.read(reinterpret_cast<char *>(&valueSize), sizeof(size_t));
        std::string value(valueSize, '\0');
        snapshotReader.read(&value[0], valueSize);

        pKVStore[key] = value;
    }
    pLruList.loadState();
    snapshotReader.close();
    // LOG THIS
    std::cout << "Snapshot Loaded Successfully" << std::endl;
}

