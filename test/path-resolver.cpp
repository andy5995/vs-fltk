#include <iostream>
#include <utils/paths.hpp>
using namespace vs;

int main(){
    std::cout<<resolve_path::normalizer("/hello world/banana/", "quick-js/././ww/aee/", true).second<<"\n";
    std::cout<<resolve_path::normalizer("/hello world/banana/", "../quick-js/ww/a", true).second<<"\n";
    std::cout<<resolve_path::normalizer("/hello world/banana/", "../../quick-js/ww/a", true).second<<"\n";
    std::cout<<resolve_path::normalizer("/hello world/banana/", "../quick-js/ww/a", false).second<<"\n";
    std::cout<<resolve_path::normalizer("/hello world/banana/", "quick-js/../ww/a", true).second<<"\n";
    std::cout<<resolve_path::normalizer("/hello world/banana/", "quick-js/../ww/a", false).second<<"\n";
    std::cout<<resolve_path::normalizer("/hello world/banana/", "./../ww/a", true).second<<"\n";
    return 0;
}