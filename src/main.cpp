#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include "KVStore.h"

const int NUM_THREADS = 100;
const int NUM_KEYS = 1000000;

// 🏋️‍♂️ Mass Insert & Get Test
void stressTestMassInsert(KVStore& kvStore) {
    std::cout << "\n🔹 [Stress Test] Massive Inserts & Gets (" << NUM_KEYS << " keys)\n";

    for (int i = 0; i < NUM_KEYS; i++) {
        kvStore.put("Key" + std::to_string(i), "Value" + std::to_string(i));
    }

    std::cout << "✅ Inserted " << NUM_KEYS << " keys. Now retrieving...\n";

    int successCount = 0;
    for (int i = 0; i < NUM_KEYS; i++) {
        if (kvStore.get("Key" + std::to_string(i)) == "Value" + std::to_string(i)) {
            successCount++;
        }
    }

    std::cout << "✅ Retrieved " << successCount << "/" << NUM_KEYS << " keys correctly.\n";
}

// 🧵 Multi-Threaded Stress Test
void stressTestConcurrency(KVStore& kvStore) {
    std::cout << "\n🔹 [Stress Test] Concurrency (" << NUM_THREADS << " threads)\n";
    std::vector<std::thread> threads;

    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back([&kvStore, i]() {
            for (int j = 0; j < 1000; j++) {  // Each thread inserts 1000 keys
                kvStore.put("Thread" + std::to_string(i) + "_Key" + std::to_string(j),
                           "Thread" + std::to_string(i) + "_Value" + std::to_string(j));
            }
        });
    }

    for (auto& thread : threads) thread.join();
    std::cout << "✅ " << NUM_THREADS << " threads completed writes.\n";

    // Verify a few random keys
    std::cout << "🔎 Verifying data integrity...\n";
    std::cout << "Thread5_Key500 -> " << kvStore.get("Thread5_Key500") << " (Expected: Thread5_Value500)\n";
}

// 🚀 LRU Stress Test (Repeated Access to Small Set of Keys)
void stressTestLRU(KVStore& kvStore) {
    std::cout << "\n🔹 [Stress Test] LRU Behavior\n";

    // Insert 2000 keys to exceed LRU max size (1000)
    for (int i = 0; i < 2000; i++) {
        kvStore.put("LRUKey" + std::to_string(i), "Value" + std::to_string(i));
    }

    std::cout << "✅ Inserted 2000 keys (should evict older ones).\n";

    // Check eviction behavior
    std::cout << "Checking if LRUKey0 is evicted -> " << kvStore.get("LRUKey0") << " (Expected: Key Not Found)\n";
    std::cout << "Checking if LRUKey1000 is still there -> " << kvStore.get("LRUKey1000") << " (Expected: Value1000)\n";
}

// 💥 Crash Recovery Test
void stressTestCrashRecovery() {
    std::cout << "\n🔹 [Stress Test] WAL Crash Recovery\n";

    {
        KVStore kvStore;
        kvStore.put("CrashTestKey", "CrashValue");
        std::cout << "✅ Inserted CrashTestKey -> CrashValue.\n";
    }  // Simulating abrupt shutdown

    std::this_thread::sleep_for(std::chrono::seconds(2));
    KVStore kvStore2;

    std::cout << "CrashTestKey after restart -> " << kvStore2.get("CrashTestKey") << " (Expected: CrashValue)\n";
}

int main() {
    KVStore kvStore;

    stressTestMassInsert(kvStore);
    stressTestConcurrency(kvStore);
    stressTestLRU(kvStore);
    stressTestCrashRecovery();

    return 0;
}