#pragma once
//Temporary cache storing files from fs, collected from remote sources or pre-compiled.
//This is not the final cache based on sqlite, but entries might be promoted under some conditions.
//This cache exists regardless of the other one being enabled or not, and has a functional purpose.

//Once the build process of an app is completed, this cache is fully released before running.
//This cache is going to be global.
#include <memory>
#include <string>
#include <sys/types.h>
#include <unordered_map>

namespace detail{
    struct key_t{
        std::string location;   //Location in fs or URI of the requested resource
        std::string local_id;   //Unique local id identifying the position of this resource in location. If left "" it represents the full file.

        bool operator==(const key_t &other) const {
            return (location == other.location
                    && local_id == other.local_id);
        }
    };
}

template <>
struct std::hash<detail::key_t>
{
std::size_t operator()(const detail::key_t& k) const
{
    return (std::hash<std::string>()(k.location)
            ^ std::hash<std::string>()(k.location));
}
};


namespace vs{

namespace cache{

/**
    Special cache class to store arbitrary data as part of the current session.
    Usually without a limited lifetime.
    It is not designed to quickly search from the original name.
 */
class memstorage_t{
    public: 
        using key_t =  detail::key_t;
        struct entry_t;
        
    private:
        std::unordered_map<key_t, entry_t> entries;
        static key_t unique_name(const char* ctx);

    public:
        enum class resource_t{
            NONE,
            DATA_CACHE,
            VS,                 //Assumed immutable
            TVS,                //Assumed immutable
            DYNLIB,             //Assumed immutable
            RESOURCE,           //Assumed immutable
            EMBEDDED_SCRIPT,    //Assumed immutable
        };

        struct entry_t{
            //std::string full_key;   //Not defined for from_shared
            std::shared_ptr<void> ref;
        };

        //struct key_t : detail::key_t{};

        struct ckey_t{
            const char* location;   //Location in fs or URI of the requested resource
            const char* local_id;   //Unique local id identifying the position of this resource in location. If left "" it represents the full file.
        };

    public:
        ckey_t fetch_from_fs(const char* path);
        ckey_t fetch_from_http(const char* path);
        ckey_t fetch_from_https(const char* path);
        ckey_t fetch_from_gemini(const char* path);
        ckey_t fetch_from_cache(const char* path);
        ckey_t fetch_from_shared(const char* path, const std::shared_ptr<void>& src);
        template<typename T> ckey_t fetch_from(const char* path, const T& src);

        void drop(ckey_t);
        entry_t* get(ckey_t);
        entry_t* get(const char* doc);
};


}

}

