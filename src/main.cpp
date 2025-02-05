#include "KVStore.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

template <typename Func>
void benchmark(const std::string& testName, Func&& function, int numThreads) {
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(function);
    }
    
    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << testName << " completed in " << duration << " ms using " 
              << numThreads << " threads.\n";
}

int main() {
    KVStore kv;

    // ✅ Pre-populate the KV store for testing
    for (int i = 0; i < 10000; ++i) {
        kv.put("key" + std::to_string(i), "value" + std::to_string(i));
    }

    std::cout << "Data Prepped" << std::endl;

    int numThreads = 10;  // Number of concurrent threads for testing

    // ✅ Benchmark GET operations
    benchmark("Read Test (get)", [&kv]() {
        for (int i = 0; i < 10000; ++i) {
            kv.get("key" + std::to_string(i % 10000));
        }
    }, numThreads);

    // ✅ Benchmark PUT operations
    benchmark("Write Test (put)", [&kv]() {
        for (int i = 0; i < 5000; ++i) {
            kv.put("key" + std::to_string(i), "updatedValue");
        }
    }, numThreads);

    // ✅ Benchmark DELETE operations
    benchmark("Delete Test (remove)", [&kv]() {
        for (int i = 0; i < 5000; ++i) {
            kv.remove("key" + std::to_string(i));
        }
    }, numThreads);

    return 0;
}