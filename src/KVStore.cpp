#include<KVStore.h>

KVStore::KVStore() {
    std::cout << "Key-Value Store Initialized" << std::endl;

    snapshot.loadSnapshot(store, lruCache, keyLocks);
    writeAheadLogger.replayWAL(store, lruCache, keyLocks);

    // std::thread([this]() {
    //     while (true) {
    //         std::this_thread::sleep_for(std::chrono::minutes(5));
    //         snapshot.saveSnapshot(store, lruCache);
    //         writeAheadLogger.flush();
    //     }
    // }).detach();
}

KVStore::~KVStore() {
    snapshot.saveSnapshot(store, lruCache);
    writeAheadLogger.flush();
}

// PUBLIC MEMBER FUNCS

void KVStore::put(const std::string& pKey, const std::string& pValue) {

    {
        std::lock_guard<std::mutex> globalLock(keyLocksMutex);
        keyLocks.try_emplace(pKey);
    }

    std::unique_lock<std::shared_mutex> lock(keyLocks[pKey]);

    store[pKey] = pValue;
    lruCache.accessKey(pKey);
    std::string evictedKey = lruCache.evictKeysIfNeeded();
    writeAheadLogger.logPut(pKey, pValue);
    lock.unlock();

    if (!evictedKey.empty()) {
        {
            std::unique_lock<std::shared_mutex> lock(keyLocks[evictedKey]);
            store.erase(evictedKey);
        }
        keyLocks.erase(evictedKey);
    }
    
}

std::string KVStore::get(const std::string& pKey) {
    std::shared_lock<std::shared_mutex> lock(keyLocks[pKey]);

    if(store.find(pKey) == store.end()) {
        return "Key Not Found";
    }

    lruCache.accessKey(pKey);
    return store[pKey];
}

void KVStore::remove(const std::string& pKey) {
    std::unique_lock<std::shared_mutex> lock(keyLocks[pKey]);
    if(store.find(pKey) != store.end()) {
        lruCache.removeKey(pKey);
        store.erase(pKey);
        writeAheadLogger.logRemove(pKey);
        lock.unlock();
        keyLocks.erase(pKey);
    }
}