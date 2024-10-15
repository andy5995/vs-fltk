#pragma once

#include "utils/paths.hpp"
#include <cstddef>
#include <cstdlib>

namespace vs{
    /**
     * @brief Fully load a file from disk to a buffer.
     * 
     * @param path A complete scoped real path.
     * @param allocator an allcoator to be used. Malloc by default.
     * @return std::pair<char *, size_t> If the string is nullptr, the second argument will have an error code.
     */
    std::pair<char *, size_t> fs_fetch_to_new(const scoped_rpath_t& path, void*(*allocator)(size_t t)=malloc);
}