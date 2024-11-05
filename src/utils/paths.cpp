#include <iostream>
#include <utils/paths.hpp>

namespace vs{

std::pair<bool, std::string> resolve_path::normalizer(const char *parent, const char *child, bool allow_exit, bool base_dir){
    int parent_len=strlen(parent);
    int child_len=strlen(child);
    char ret[parent_len+child_len+1];
    memset(ret+parent_len,0,child_len+1);
    memcpy(ret,parent,parent_len);
    int ptr = parent_len;
    
    for(int j=0; j<child_len; ){
        if(tkn(child+j,"../")){
            //Go back to the past /
            if(ptr<=1)return {false, ""}; //Early failure, cannot track back.
            ptr-=2;
            for(;ret[ptr]!='/';ptr--);
            j+=3;
            ptr++;
            if(ptr<parent_len && allow_exit==false){return {false, ""};} //I reached the parent and cannot get any further.
            continue;
        }
        else if(tkn(child+j,"./")){
            j+=2;   //Just skip this.
            continue;
        }
        else if(child[j]=='/' && child[j+1]=='/'){
            j++;
            continue;
        }
        else if(child[j]=='/'){
            j++;
        }
        else{
            ret[ptr]=child[j];
            ptr++;
            j++;
            continue;
        }

        if(j!=child_len){
            ret[ptr]='/';
            ptr++;
        }
    }

    //TODO: Stress test for potential unsafety
    if(base_dir){
        for(;ret[ptr]!='/';ptr--);
        ptr++;
    }    
    
    ret[ptr]=0;

    return {true,std::string(ret)};
}

//TODO: Once policies are implemented, they will be tested in here as well.
std::pair<resolve_path::reason_t::t,scoped_rpath_t> resolve_path::operator()(from_t from,const char* src){
    scoped_vpath_t tmp; tmp.from_string(src);

    if(tmp.type==vpath_type_t::THIS){
        auto normalized = normalizer(local.location.data(), tmp.location.data(), false);
        if(normalized.first){
            return {reason_t::OK,{local.type,normalized.second}};
        }
        else return {reason_t::ROOT_REACHED, {rpath_type_t::NONE, ""}};
    }
    //All on FS Block
    else if(tmp.type==vpath_type_t::CWD){
        auto normalized = normalizer(env.cwd.location.data(), tmp.location.data(), false);
        if(normalized.first){
            return {reason_t::OK,{env.cwd.type,normalized.second}};
        }
        else return {reason_t::ROOT_REACHED, {rpath_type_t::NONE, ""}};
    }
    else if(tmp.type==vpath_type_t::REPO){
        auto normalized = normalizer(env.packages_path.location.data(), tmp.location.data(), false);
        if(normalized.first){
            return {reason_t::OK,{env.packages_path.type,normalized.second}};
        }
        else return {reason_t::ROOT_REACHED, {rpath_type_t::NONE, ""}};
    }
    else if(tmp.type==vpath_type_t::DATA){
        auto normalized = normalizer(env.appdata_path.location.data(), tmp.location.data(), false);
        if(normalized.first){
            return {reason_t::OK,{env.appdata_path.type,normalized.second}};
        }
        else return {reason_t::ROOT_REACHED, {rpath_type_t::NONE, ""}};
    }
    else if(tmp.type==vpath_type_t::TMP){
        auto normalized = normalizer(env.tmp_path.location.data(), tmp.location.data(), false);
        if(normalized.first){
            return {reason_t::OK,{env.tmp_path.type,normalized.second}};
        }
        else return {reason_t::ROOT_REACHED, {env.tmp_path.type, ""}};
    }
    else if(tmp.type==vpath_type_t::VS){
        auto normalized = normalizer(env.app_path.location.data(), tmp.location.data(), false);
        if(normalized.first){
            return {reason_t::OK,{rpath_type_t::FS,normalized.second}};
        }
        else return {reason_t::ROOT_REACHED, {rpath_type_t::NONE, ""}};
    }
    else if(tmp.type==vpath_type_t::FS){
        if(from==from_t::NATIVE_CODE || policies.scripts.allow_fs)
            return {reason_t::OK, {rpath_type_t::FS, tmp.location}};
        else return {reason_t::POLICY_VIOLATION, {rpath_type_t::NONE, ""}};
    }
    else if(tmp.type==vpath_type_t::APP){
        auto normalized = normalizer(env.root.location.data(), tmp.location.data(), false);
        if(normalized.first){
            return {reason_t::OK,{env.root.type,normalized.second}};
        }
        else return {reason_t::ROOT_REACHED, {rpath_type_t::NONE, ""}};
    }
    #ifdef HAS_CURL
    else if(tmp.type==vpath_type_t::HTTP){
        if(((from==from_t::NATIVE_CODE ) || (from==from_t::EMBEDDED_SCRIPT && policies.scripts.allow_networking)) && policies.networking.allow_http)
            return {reason_t::OK, {rpath_type_t::HTTP, tmp.location}};
        else return {reason_t::POLICY_VIOLATION, {rpath_type_t::NONE, ""}};
    }
    else if(tmp.type==vpath_type_t::HTTPS){
        if(((from==from_t::NATIVE_CODE) || (from==from_t::EMBEDDED_SCRIPT && policies.scripts.allow_networking)) && policies.networking.allow_https){
            return {reason_t::OK, {rpath_type_t::HTTPS, tmp.location}};
        }
        else return {reason_t::POLICY_VIOLATION, {rpath_type_t::NONE, ""}};
    }
    else if(tmp.type==vpath_type_t::GEMINI){
        if(((from==from_t::NATIVE_CODE) || (from==from_t::EMBEDDED_SCRIPT && policies.scripts.allow_networking)) && policies.networking.allow_gemini)
            return {reason_t::OK, {rpath_type_t::GEMINI, tmp.location}};
        else return {reason_t::POLICY_VIOLATION, {rpath_type_t::NONE, ""}};
    }
    #endif
    return {reason_t::MALFORMED,{rpath_type_t::NONE, ""}};
}  

}