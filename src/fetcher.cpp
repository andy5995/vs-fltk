#include <cache/memory-storage.hpp>
#include <fetcher.hpp>

namespace vs{

component_t component_t_i(const char* t){
    if(false);
    else if(strcmp(t,".vs")==1)return component_t::VS;
    else if(strcmp(t,".xml")==1)return component_t::XML;
    else if(strcmp(t,".wasm")==1)return component_t::WASM;
#   if defined(__linux__)
    else if(strcmp(t,".so")==1)return component_t::LIB;
#   elif defined(_WIN32) || defined(_WIN64)
    else if(strcmp(t,".dll")==1)return component_t::LIB;
#   elif defined(__APPLE__)
    else if(strcmp(t,".dylib")==1)return component_t::LIB;
#   endif
    else if(strcmp(t,".c")==1)return component_t::CNATIVE;
    else if(strcmp(t,".md")==1)return component_t::MARKDOWN;
    else return component_t::NONE;
}

constexpr const char* component_t_s(component_t t){
    if(t==component_t::NONE)return nullptr;
    else if(t==component_t::VS)return ".vs";
    else if(t==component_t::XML)return ".xml";
    else if(t==component_t::WASM)return ".wasm";
    else if(t==component_t::LIB){
#   if defined(__linux__)
        return ".so";
#   elif defined(_WIN32) || defined(_WIN64)
        return ".dll";
#   elif defined(__APPLE__)
        return ".dylib";
#   endif
    }
    else if(t==component_t::CNATIVE)return ".c";
    else if(t==component_t::MARKDOWN)return ".md";
    else return nullptr;
}

std::tuple<resolve_path::reason_t::t, cache::mem_storage_t::entry_it*, component_t> fetch_component(){
    //TODO:
}

std::tuple<resolve_path::reason_t::t,cache::buffer_t, scoped_rpath_t> fetcher(resolve_path& base, resolve_path::from_t from,const char* src, bool promote, bool preserve){
    auto ret = base(from,src);
    if(ret.first==resolve_path::reason_t::OK){

        //If present, use the one cached.
        {
            auto found = globals::mem_storage.get({ret.second.location,0,cache::resource_t::BUFFER});
            if(found!=nullptr){
                cache::buffer_t bf = * (cache::buffer_t*)found->ref.get();
                return {resolve_path::reason_t::OK,bf,ret.second};
            }
        }

        if(ret.second.type==rpath_type_t::FS){
            auto res = globals::mem_storage.fetch_from_fs({ret.second.location,0,cache::resource_t::BUFFER,promote,preserve});
            if(res==globals::mem_storage.end()){
                return {resolve_path::reason_t::NOT_FOUND,{nullptr,0},ret.second};
            }
            else{
                cache::buffer_t bf = * (cache::buffer_t*)res->second.ref.get();
                return {resolve_path::reason_t::OK,bf,ret.second};
            }
        }
#       ifdef HAS_CURL
        else if(ret.second.type==rpath_type_t::HTTP){
            auto res = globals::mem_storage.fetch_from_http({ret.second.as_string(),0,cache::resource_t::BUFFER,promote,preserve});
            if(res==globals::mem_storage.end()){
                return {resolve_path::reason_t::NOT_FOUND,{nullptr,0},ret.second};
            }
            else{
                cache::buffer_t bf = * (cache::buffer_t*)res->second.ref.get();
                return {resolve_path::reason_t::OK,bf,ret.second};
            }
        }
        else if(ret.second.type==rpath_type_t::HTTPS){
            auto res = globals::mem_storage.fetch_from_https({ret.second.as_string(),0,cache::resource_t::BUFFER,promote,preserve});
            if(res==globals::mem_storage.end()){
                return {resolve_path::reason_t::NOT_FOUND,{nullptr,0},ret.second};
            }
            else{
                cache::buffer_t bf = * (cache::buffer_t*)res->second.ref.get();
                return {resolve_path::reason_t::OK,bf,ret.second};
            }
        }
#       endif
        else return {resolve_path::reason_t::NOT_IMPLEMENTED, {nullptr,0},ret.second};
    }
    else return {ret.first,{nullptr,0},ret.second};
}

std::tuple<resolve_path::reason_t::t,const pugi::xml_document*, scoped_rpath_t> fetcher_xml(resolve_path& base, resolve_path::from_t from,const char* src, bool promote, bool preserve){
    //TODO: implement
}


}