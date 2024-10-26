#pragma once
//Temporary cache storing files from fs, collected from remote sources or pre-compiled.
//This is not the final cache based on sqlite, but entries might be promoted under some conditions.
//This cache exists regardless of the other one being enabled or not, and has a functional purpose.

//Once the build process of an app is completed, this cache is fully released before running.
//This cache is going to be global.
#include <memory>
#include <string>
#include <unordered_map>

namespace vs{

namespace cache{

class memstorage{
    struct entry_t{
        std::string full_key;
        std::shared_ptr<void> ref;
    };

    std::unordered_map<std::string, entry_t> entries;

    public:
        entry_t* fetch_from_fs(const char* path);
        entry_t* fetch_from_http(const char* path);
        entry_t* fetch_from_https(const char* path);
        entry_t* fetch_from_gemini(const char* path);
        entry_t* fetch_from_cache(const char* path);
        entry_t* fetch_from_shared(const std::shared_ptr<void>& src);

        void drop(const char* key);
        entry_t* get(const char* key);
};

}

}