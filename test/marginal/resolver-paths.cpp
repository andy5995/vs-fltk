#include <iostream>
#include <utils/paths.hpp>
#include <cassert>

using namespace vs;

int test_normalizer(const char* expected, const char* parent, const char* child, bool allow_exit, bool base_dir){
    auto tmp = resolve_path::normalizer(parent,child,allow_exit,base_dir);
    std::cout<<"╭<parent> "<<parent<<"\n";
    std::cout<<"├<child>  "<<child<<"\n";
    std::cout<<"├>        "<<tmp.second<<"\n";
    std::cout<<"╰>>       "<<expected<<"\n";
    //if(!tmp.first)return 1;
    if(tmp.second==expected){return 0;}
    else{
        return 1;
    }
}

int main(){
    int ret = 0;
    ret+=test_normalizer("/hello world/banana/quick-js/ww/aee/", "/hello world/banana/", "quick-js/././ww/aee/", true, false);
    ret+=test_normalizer("/hello world/banana/quick-js/ww/aee/", "/hello world/banana/", "quick-js/././ww/aee/", true, true);
    ret+=test_normalizer("/hello world/banana/quick-js/ww/", "/hello world/banana/", "quick-js/././ww/aee", true, true);
    ret+=test_normalizer("/quick-js/ww/a", "/hello world/banana/", "../../quick-js/ww/a", true, false);
    ret+=test_normalizer("/quick-js/ww/a", "/hello world/banana/", "./../.././quick-js/ww/a", true, false);
    ret+=test_normalizer("", "/hello world/banana/", "../../quick-js/ww/a", false, false);
    ret+=test_normalizer("", "/hello world/banana/", "./../.././quick-js/ww/a", false, false);
    ret+=test_normalizer("/hello world/banana/ww/a", "/hello world/banana/", "quick-js/../quick-js/ww/a", false, false);
    ret+=test_normalizer("/hello world/banana/ww/a", "/hello world/banana/", "quick-js/./.././ww/a", false, false);
    ret+=test_normalizer("/a/b../", "/a/", "b../", false, false);

    assert(ret=0);
    return 0;
}