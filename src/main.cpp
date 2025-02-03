#include<KVStore.h>

int main() {
    KVStore kv;

    std::cout << "age: " << kv.get("age") << std::endl;

    kv.put("name", "Suhas");
    kv.put("age", "26");

    std::cout << "name: " << kv.get("name") << std::endl;
    std::cout << "age: " << kv.get("age") << std::endl;

    kv.remove("name");
    std::cout << "After deletion, name: " << kv.get("name") << std::endl;

    return 0;
}