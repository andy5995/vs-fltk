#include "cache/memory-storage.hpp"
#include "pipelines/quickjs-js.hpp"
#include <fetcher.hpp>

namespace vs{

std::tuple<resolve_path::reason_t::t,cache::buffer_t, scoped_rpath_t> fetcher(resolve_path& base, resolve_path::from_t from,const char* src, bool promote, bool preserve){
    auto ret = base(from,src);
    if(ret.first==resolve_path::reason_t::OK){

        //If present, use the one cached.
        {
            auto found = globals::memstorage.get({ret.second.location,0,cache::resource_t::BUFFER});
            if(found!=nullptr){
                cache::buffer_t bf = * (cache::buffer_t*)found->ref.get();
                return {resolve_path::reason_t::OK,bf,ret.second};
            }
        }

        if(ret.second.type==rpath_type_t::FS){
            auto res = globals::memstorage.fetch_from_fs({ret.second.location,0,cache::resource_t::BUFFER,promote,preserve});
            if(res==globals::memstorage.end()){
                return {resolve_path::reason_t::NOT_FOUND,{nullptr,0},ret.second};
            }
            else{
                cache::buffer_t bf = * (cache::buffer_t*)res->second.ref.get();
                return {resolve_path::reason_t::OK,bf,ret.second};
            }
        }
#       ifdef HAS_CURL
        else if(ret.second.type==rpath_type_t::HTTP){
            auto res = globals::memstorage.fetch_from_http({ret.second.as_string(),0,cache::resource_t::BUFFER,promote,preserve});
            if(res==globals::memstorage.end()){
                return {resolve_path::reason_t::NOT_FOUND,{nullptr,0},ret.second};
            }
            else{
                cache::buffer_t bf = * (cache::buffer_t*)res->second.ref.get();
                return {resolve_path::reason_t::OK,bf,ret.second};
            }
        }
        else if(ret.second.type==rpath_type_t::HTTPS){
            auto res = globals::memstorage.fetch_from_https({ret.second.as_string(),0,cache::resource_t::BUFFER,promote,preserve});
            if(res==globals::memstorage.end()){
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