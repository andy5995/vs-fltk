#pragma once

#include <cstddef>

namespace vs{

struct policies_t{
enum class script_safety_t{
    UNSAFE, //Native targets are enabled
    SAFER,  //Native targets are disabled, but quickjs is not
    SAFE,   //Only WASM allowed, but trusted native components can be used too
    SAFEST  //Only WASM is allowed
}script_safety;
enum class data_safety_t{
    UNSAFE,     //Full access to the entire fs
    RESTRICTED, //Access restricted to the local folders where the application is run & libraries
    SCOPED      //Access restricted to the basename. No .. allowed and no absolute path, only this:// without ...
}data_safety;
size_t max_depth;   //Maximum nesting in components to avoid excessive memory usage in case of component loops. 
enum class http_t{
    HTTP_ALLOWED,
    HTTPS_ONLY,
    AIRGAP_ONLY
}network;
//std::vector<std::string> trusted_domains;

//Return true if this is a good derivation from parent. Else false.
//It is used to invalidate changes which are against root wishes.
bool fix_derivation(const policies_t& parent){
    bool is_good = true;
    if(script_safety>parent.script_safety){is_good=false;script_safety=parent.script_safety;}
    if(data_safety>parent.data_safety){is_good=false;data_safety=parent.data_safety;}
    if(max_depth>parent.max_depth){is_good=false;max_depth=parent.max_depth;}
    if(network > parent.network){is_good=false;network=parent.network;}
    //TODO:Checking trusted domain is VERY hard and a different data structure would be better.
    //Like a tree or something.
    return is_good;
}
};

}