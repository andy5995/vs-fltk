#pragma once
//Temporary cache storing files from fs, remote sources or pre-compiled.
//This is not the final cache based on sqlite, but entries might be promoted under some conditions.
//This cache exists regardless of the other types being enabled or not, and has a functional purpose to support the xml parsing.

//Once the build process of an app is completed, its content is fully before running (with a flag for those objects which must persist).
//This cache is exposed as a global singleton.

//Its content is not exposed via any sort of API to embedded scripts.

#include <utils/strings.hpp>
#include <ui-frame-public.hpp>
#include <cstddef>
#include <memory>
#include <string>
#include <sys/types.h>
#include <unordered_map>

namespace vs{
namespace cache{

struct script_t{
  std::shared_ptr<void> script;
  std::shared_ptr<smap<symbol_t>> symbols;
  frame_mode_t mode;
};

struct buffer_t{
    const uint8_t* data;
    size_t size;
};


enum class resource_t{
            NONE,
            BUFFER,                     //uint8_t[]
            XML_TREE,                   //pugi::xml_document
            SCRIPT,                     //struct script_t
            DYNAMIC_LIB,                //?
            OTHER_RESOURCE,             //?
};

struct key_t{
    std::string location;   //Location in fs or URI of the requested resource
    size_t local_id:16=0;     //Unique local id identifying the position of this resource in location. If left "" it represents the full file.
    resource_t resource:4=resource_t::NONE;
    size_t to_promote:1=false;      //Consider for promotion to higher tier cache
    size_t to_keep:1=true;         //To be kept at runtime.

    bool operator==(const key_t &other) const {
        return (location == other.location
                && local_id == other.local_id && resource==other.resource);
    }
};
}
}

template <>
struct std::hash<vs::cache::key_t>
{
std::size_t operator()(const vs::cache::key_t& k) const
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
        struct entry_t{
            std::shared_ptr<void> ref;
        };
        typedef std::unordered_map<key_t, entry_t>::const_iterator entry_it;
    private:
        std::unordered_map<key_t, entry_t> entries;
        static key_t unique_name(const char* ctx);

    public:
        inline entry_it end() {return entries.end();}
        static inline key_t empty_key = {"",0,resource_t::NONE};
    public:

        entry_it fetch_from_fs(const key_t& path);
#       ifdef HAS_CURL
            entry_it fetch_from_http(const key_t& path);
            entry_it fetch_from_https(const key_t& path);
            entry_it fetch_from_gemini(const key_t& path);
#       endif
        entry_it fetch_from_cache(const key_t& path);
        entry_it fetch_from_shared(const key_t& key, const std::shared_ptr<void>& src);


        void drop(const key_t&);
        
        entry_t* get(const key_t&);

        //Remove all entries which are no longer needed (be it because already consumed, or because copies of shared_ptr are all in place)
        void cleanup();
};


}

}

