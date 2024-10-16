#include "ui.hpp"
#include <cstdlib>
#include <cstring>
#include <utils/policies.hpp>

namespace vs{

void policies_t::inherit_from_env(){
    const char* VS_GLOBAL_POLICY = getenv("VS_GLOBAL_POLICY");
    if(VS_GLOBAL_POLICY==nullptr) {normal();}
    else if(strcmp(VS_GLOBAL_POLICY,"all")==0){all();}
    else if(strcmp(VS_GLOBAL_POLICY,"none")==0){none();}
    else if(strcmp(VS_GLOBAL_POLICY,"trusted")==0){trusted();}
    else if(strcmp(VS_GLOBAL_POLICY,"normal")==0){normal();}
    else if(strcmp(VS_GLOBAL_POLICY,"safe")==0){safe();} 
    else {vs_log(severety_t::WARNING,nullptr,"Unrecognized VS_GLOBAL_POLICY `%s`, defaulting to `normal`",VS_GLOBAL_POLICY);}

    const char* VS_NETWORKING_POLICY = getenv("VS_NETWORKING_POLICY");
    if(VS_NETWORKING_POLICY==nullptr) {normal();}
    else if(strcmp(VS_NETWORKING_POLICY,"all")==0){networking.all();}
    else if(strcmp(VS_NETWORKING_POLICY,"none")==0){networking.none();}
    else if(strcmp(VS_NETWORKING_POLICY,"trusted")==0){networking.trusted();}
    else if(strcmp(VS_NETWORKING_POLICY,"normal")==0){networking.normal();}
    else if(strcmp(VS_NETWORKING_POLICY,"safe")==0){networking.safe();}
    else {vs_log(severety_t::WARNING,nullptr,"Unrecognized VS_NETWORKING_POLICY `%s`, skipping",VS_NETWORKING_POLICY);}

    const char* VS_SCRIPTS_POLICY = getenv("VS_SCRIPTS_POLICY");
    if(VS_SCRIPTS_POLICY==nullptr) {normal();}
    else if(strcmp(VS_SCRIPTS_POLICY,"all")==0){embedded_scripts.all();}
    else if(strcmp(VS_SCRIPTS_POLICY,"none")==0){embedded_scripts.none();}
    else if(strcmp(VS_SCRIPTS_POLICY,"trusted")==0){embedded_scripts.trusted();}
    else if(strcmp(VS_SCRIPTS_POLICY,"normal")==0){embedded_scripts.normal();}
    else if(strcmp(VS_SCRIPTS_POLICY,"safe")==0){embedded_scripts.safe();}
    else {vs_log(severety_t::WARNING,nullptr,"Unrecognized VS_SCRIPTS_POLICY `%s`, skipping",VS_SCRIPTS_POLICY);}

    const char* VS_NATIVE_COMPONENTS_POLICY = getenv("VS_SCRIPTS_POLICY");
    if(VS_NATIVE_COMPONENTS_POLICY==nullptr) {normal();}
    else if(strcmp(VS_NATIVE_COMPONENTS_POLICY,"all")==0){allow_native_components=true;}
    else if(strcmp(VS_NATIVE_COMPONENTS_POLICY,"none")==0){allow_native_components=false;}
    else {vs_log(severety_t::WARNING,nullptr,"Unrecognized VS_NATIVE_COMPONENTS_POLICY `%s`, skipping",VS_NATIVE_COMPONENTS_POLICY);}

    const char* VS_LOG_LEVEL = getenv("VS_LOG_LEVEL");
    if(VS_LOG_LEVEL==nullptr) {normal();}
    else if(strcmp(VS_LOG_LEVEL,"silent")==0){verbosity=0;}
    else if(strcmp(VS_LOG_LEVEL,"basic")==0){verbosity=3;}
    else if(strcmp(VS_LOG_LEVEL,"verbose")==0){verbosity=7;}
    else {vs_log(severety_t::WARNING,nullptr,"Unrecognized VS_LOG_LEVEL `%s`, skipping",VS_LOG_LEVEL);}

    //The rest are not considered as ENV variables have a limited granularity
}

void policies_t::inherit_from_xml(const pugi::xml_node& root){
    //TODO: Load from a profile
}

void policies_t::save_to_xml(pugi::xml_node root){
    //TODO: Save as part of a profile
}

}