cmake_minimum_required(VERSION 3.10)
project(HighPerfKVStore)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(HighPerfKVStore src/main.cpp src/KVStore.cpp src/LRUCache.cpp src/Snapshot.cpp src/WAL.cpp)