#include <cstdio>
#include <globals.hpp>
#include <cstdlib>
#include <chrono>

namespace vs{
namespace globals{
    
policies_t  policy;
path_env_t  path_env;
js_rt_t     js_rt;
cache::mem_storage_t mem_storage;

vs_test_debug_t debug;

vs_test_debug_t::vs_test_debug_t(){auto file=getenv("VS_DEBUG_FILE");if(file!=nullptr)fd=fopen(file,"w+");}
vs_test_debug_t::~vs_test_debug_t(){if(fd!=nullptr)fclose(fd);}

void vs_test_debug_t::operator()(const char* field, const char* value){  
    if(fd==nullptr)return;
    else{
        auto now = std::chrono::system_clock::now();
        fprintf(fd,"%s\t%s\t%ld\n",field,value,std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count());
    }
}

//TODO: Fill in with the basic types supported.
value_models_t value_models;


}
}
