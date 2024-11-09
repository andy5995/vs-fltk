#include <cstdio>
#include <globals.hpp>
#include <cstdlib>

namespace vs{
namespace globals{
    
policies_t  policy;
path_env_t  path_env;
js_rt_t     js_rt;
cache::memstorage_t memstorage;

vs_test_debug_t vs_test_debug;

vs_test_debug_t::vs_test_debug_t(){auto file=getenv("VS_TEST_DEBUG");if(file!=nullptr)fd=fopen(file,"w+");}
vs_test_debug_t::~vs_test_debug_t(){if(fd!=nullptr)fclose(fd);}

void vs_test_debug_t::operator()(const char* field, const char* value){  
    if(fd==nullptr)return;
    else{
        fprintf(fd,"%s\t%s",field,value);
    }
}

}
}
