#include <utils/paths.hpp>
#include <resolvers/network.hpp>
#include <ui.hpp>

//TODO: Avoid directories& symlinks
namespace vs{
    #ifdef HAS_CURL
    std::pair<char *, size_t> http_fetch_to_new(const scoped_rpath_t& path, void*(*allocator)(size_t t)){
        vs_log(severety_t::WARNING, nullptr, "HTTP/HTTPS not implemented yet.");
        return {nullptr,0};
    }
    std::pair<char *, size_t> https_fetch_to_new(const scoped_rpath_t& path, void*(*allocator)(size_t t)){
        vs_log(severety_t::WARNING, nullptr, "HTTP/HTTPS not implemented yet.");
        return {nullptr,0};
    }
    #else
    std::pair<char *, size_t> http_fetch_to_new(const scoped_rpath_t& path, void*(*allocator)(size_t t)){
        vs_log(severety_t::WARNING, nullptr, "Remote file `%s` was requested, but `vs` is built without `libcurl`.", path.as_string().data());
        return {nullptr,0};
    }
    std::pair<char *, size_t> https_fetch_to_new(const scoped_rpath_t& path, void*(*allocator)(size_t t)){
        vs_log(severety_t::WARNING, nullptr, "Remote file `%s` was requested, but `vs` is built without `libcurl`.", path.as_string().data());
        return {nullptr,0};
    }
    #endif
}