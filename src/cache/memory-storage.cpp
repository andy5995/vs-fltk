#include <cache/memory-storage.hpp>
#include <memory>
#include <sys/types.h>
#include <unordered_map>

namespace vs{

namespace cache{


const key_t&  memstorage_t::fetch_from_fs(const key_t& path){
    return empty_key;
}

const key_t&  memstorage_t::fetch_from_http(const key_t& path){
    return empty_key;
}

const key_t& memstorage_t::fetch_from_https(const key_t& path){
    return empty_key;
}

const key_t&  memstorage_t::fetch_from_gemini(const key_t& path){
    return empty_key;
}

const key_t& memstorage_t::fetch_from_cache(const key_t& path){
    return empty_key;
}

const key_t& memstorage_t::fetch_from_shared(const key_t& key, const std::shared_ptr<void>& src){
    key_t k = {key.location,key.local_id};
    auto it = entries.emplace(k,src);
    if(it.second==true)return it.first->first;
    else return empty_key;
;
}

void memstorage_t::drop(const key_t& key){
    entries.erase({key.location,key.local_id});
}

//Search fragment inside document
memstorage_t::entry_t* memstorage_t::get(const key_t& key){
    auto it = entries.find({key.location,key.local_id, key.resource});
    if(it!=entries.end())return &it->second;
    else return nullptr;
}

void memstorage_t::cleanup(){
    for(auto i =this->entries.begin();i!=this->entries.end();){
        if(i->first.to_keep==false) i = this->entries.erase(i);
        else i++;
    }
}

}

}