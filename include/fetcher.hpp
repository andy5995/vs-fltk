#pragma once
#include "cache/memory-storage.hpp"
#include <cstdint>
#include <utils/paths.hpp>
#include <globals.hpp>

namespace vs{

std::tuple<resolve_path::reason_t::t,cache::buffer_t, scoped_rpath_t> fetcher(resolve_path& base, resolve_path::from_t from,const char* src,  cache::resource_t restype=cache::resource_t::BUFFER, bool promote=false, bool preserve=false);

}