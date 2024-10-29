#include <cache/memory-storage.hpp>

namespace vs{

namespace cache{


const char*  memstorage_t::fetch_from_fs(const char* path){

}

const char*  memstorage_t::fetch_from_http(const char* path){

}

const char*  memstorage_t::fetch_from_https(const char* path){

}

const char*  memstorage_t::fetch_from_gemini(const char* path){

}

const char* memstorage_t::fetch_from_cache(const char* path){

}

const char* memstorage_t::fetch_from_shared(const std::shared_ptr<void>& src){
    auto it = entries.emplace(unique_name(),src);
    if(it.second==true)return it.first->first.c_str();
    else return nullptr;
}

void memstorage_t::drop(const char* key){
    entries.erase(key);
}

memstorage_t::entry_t* memstorage_t::get(const char* key){
    auto it = entries.find(key);
    if(it!=entries.end())return &it->second;
    else return nullptr;
}

std::string memstorage_t::unique_name(){
    //TODO: Apply a more robust way to handle unique names with some degree of hashing.
    static int counter = 0;
    counter++;
    return std::to_string(counter);
}



}

}