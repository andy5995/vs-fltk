#pragma once

#include "utils/paths.hpp"
#include <cstddef>
#include <cstdlib>

namespace vs{
    std::pair<char *, size_t> http_fetch_to_new(const scoped_rpath_t& path, void*(*allocator)(size_t t)=malloc);
    std::pair<char *, size_t> https_fetch_to_new(const scoped_rpath_t& path, void*(*allocator)(size_t t)=malloc);
}