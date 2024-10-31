#include <cache/memory-storage.hpp>
#include <memory>
#include <unordered_map>

namespace vs{

namespace cache{


memstorage_t::ckey_t  memstorage_t::fetch_from_fs(const char* path){

}

memstorage_t::ckey_t  memstorage_t::fetch_from_http(const char* path){

}

memstorage_t::ckey_t memstorage_t::fetch_from_https(const char* path){

}

memstorage_t::ckey_t  memstorage_t::fetch_from_gemini(const char* path){

}

memstorage_t::ckey_t memstorage_t::fetch_from_cache(const char* path){

}

memstorage_t::ckey_t memstorage_t::fetch_from_shared(const char* path, const std::shared_ptr<void>& src){
    auto it = entries.emplace(unique_name(path),src);
    if(it.second==true)return {it.first->first.location.c_str(),it.first->first.local_id.c_str()};
    else return {nullptr,nullptr};
}

template<typename T> memstorage_t::ckey_t memstorage_t::fetch_from(const char* path, const T& src){
    auto shared = std::make_shared(src);
    auto it = entries.emplace(unique_name(path),shared);
    if(it.second==true)return {it.first->first.location.c_str(),it.first->first.local_id.c_str()};
    else return {nullptr,nullptr};
}


void memstorage_t::drop(memstorage_t::ckey_t key){
    entries.erase({key.location,key.local_id});
}

//Search fragment into document
memstorage_t::entry_t* memstorage_t::get(memstorage_t::ckey_t key){
    auto it = entries.find({key.location,key.local_id});
    if(it!=entries.end())return &it->second;
    else return nullptr;
}

//Search document
memstorage_t::entry_t* memstorage_t::get(const char* key){
    auto it = entries.find({key,""});
    if(it!=entries.end())return &it->second;
    else return nullptr;
}

memstorage_t::key_t memstorage_t::unique_name(const char* ctx){
    //TODO: Apply a more robust way to handle unique names with some degree of hashing.
    static std::unordered_map<std::string, int> counter = {};
    auto it =counter.find(ctx);
    if(it!=counter.end())*it++;
    else it=counter.insert({ctx,0}).first;
    return {ctx, std::to_string(it->second)};
}



}

}