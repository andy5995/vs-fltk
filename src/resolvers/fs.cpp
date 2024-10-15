#include <utils/paths.hpp>
#include <resolvers/fs.hpp>

//TODO: Avoid directories& symlinks
namespace vs{
    std::pair<char *, size_t> fs_fetch_to_new(const scoped_rpath_t& path, void*(*allocator)(size_t t)){
        if(path.type!=rpath_type_t::FS)return {nullptr,0};

        char* buffer = nullptr;
        long fsize = 0;
        {
            FILE *f = fopen(path.location.data(), "rb");
            if(f!=nullptr){
            fseek(f, 0, SEEK_END);
            fsize = ftell(f);
            fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

            buffer = static_cast<char*>(allocator(fsize+1));

            fread(buffer, fsize, 1, f);
            fclose(f);
            buffer[fsize] = 0;
            
            }
            else return {nullptr, 0};
        }
        return {buffer,fsize};
    }
}