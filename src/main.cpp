#include <iostream>
#include "KVStore.h"

void testKVStore() {
    {
        KVStore kvStore;

        std::cout << "\n--- Inserting key-value pairs ---\n";
        kvStore.put("apple", "red");
        kvStore.put("banana", "yellow");
        kvStore.put("grape", "purple");

        std::cout << "apple -> " << kvStore.get("apple") << std::endl;
        std::cout << "banana -> " << kvStore.get("banana") << std::endl;
        std::cout << "grape -> " << kvStore.get("grape") << std::endl;

        std::cout << "\n--- Testing update ---\n";
        kvStore.put("banana", "green");
        std::cout << "banana (updated) -> " << kvStore.get("banana") << std::endl;

        std::cout << "\n--- Testing removal ---\n";
        kvStore.remove("apple");
        std::cout << "apple -> " << kvStore.get("apple") << std::endl;  // Should return "Key Not Found"

        std::cout << "\n--- Testing LRU Cache Eviction ---\n";
        for (int i = 0; i < 8; i++) {
            kvStore.put("key" + std::to_string(i), "value" + std::to_string(i));
        }
        std::cout << "grape -> " << kvStore.get("grape") << std::endl; 
    }

    std::cout << "\n--- Restarting KVStore to test persistence ---\n";
    KVStore kvStore2;
    std::cout << "banana -> " << kvStore2.get("banana") << std::endl;  // Should be "green" if persistence works
}

int main() {
    testKVStore();
    return 0;
}