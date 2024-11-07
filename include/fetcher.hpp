#pragma once
#include "cache/memory-storage.hpp"
#include <cstdint>
#include <utils/paths.hpp>
#include <globals.hpp>

namespace vs{

/**
 * @brief Fetch resource using its path and some contextual information (or get it if already on cache)
 * 
 * @param base The current base path on which `src` is computed
 * @param scope If this request is being performed from vs-fltk code or from an embedded script 
 * @param src the path of the source file
 * @param promote if true, this entry should be promoted to higher tier cache once the build process is done
 * @param preserve if true, this entry should be preserved after the build process is done
 * @return std::tuple<resolve_path::reason_t::t,cache::buffer_t, scoped_rpath_t> the return code, a buffer and the new base path derived from src
 */
std::tuple<resolve_path::reason_t::t,cache::buffer_t, scoped_rpath_t> fetcher(resolve_path& base, resolve_path::from_t scope,const char* src, bool promote=false, bool preserve=false);

/**
 * @brief Fetch a resource, convert it in an xml tree and store in in cache (or get it if already there)
 * @details This is to be used in place of `fetch` as it avoids storing temporary buffers, ensuring max performance for XML parsing.
 * 
 * @param base The current base path on which `src` is computed
 * @param scope If this request is being performed from vs-fltk code or from an embedded script 
 * @param src the path of the source file
 * @param promote if true, this entry should be promoted to higher tier cache once the build process is done
 * @param preserve if true, this entry should be preserved after the build process is done
 * @return std::tuple<resolve_path::reason_t::t,cache::xml_tree_t, scoped_rpath_t> the return code, a buffer and the new base path derived from src
 */
std::tuple<resolve_path::reason_t::t,cache::xml_tree_t, scoped_rpath_t> fetcher_xml(resolve_path& base, resolve_path::from_t scope,const char* src, bool promote=false, bool preserve=false);

}