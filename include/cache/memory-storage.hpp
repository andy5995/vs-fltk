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

/**
    Special cache class to store arbitrary data as part of the current session.
    Usually without a limited lifetime.
    It is not designed to quickly search from the original name.
 */
class memstorage_t{
    struct entry_t{
        //std::string full_key;   //Not defined for from_shared
        std::shared_ptr<void> ref;
    };

    std::unordered_map<std::string, entry_t> entries;

    static std::string unique_name();

    public:
        const char* fetch_from_fs(const char* path);
        const char* fetch_from_http(const char* path);
        const char* fetch_from_https(const char* path);
        const char* fetch_from_gemini(const char* path);
        const char* fetch_from_cache(const char* path);
        const char* fetch_from_shared(const std::shared_ptr<void>& src);

        void drop(const char* key);
        entry_t* get(const char* key);
};

}

}