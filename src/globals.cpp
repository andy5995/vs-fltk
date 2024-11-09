#include <globals.hpp>
namespace vs{
namespace globals{
    
policies_t  policy;
path_env_t  path_env;
js_rt_t     js_rt;
cache::memstorage_t memstorage;

FILE* vs_test_debug_fd=nullptr;
void vs_test_debug(const char* field, const char* value){
    if(vs_test_debug_fd==nullptr)return;
    else{
        fprintf(vs_test_debug_fd,"%s\t%s",field,value);
    }
}

}
}
