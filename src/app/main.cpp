
/***
 *                                                            
 *     _     _   _____          ______  _    _______  _    _  
 *    (_)   (_) (_____)        (______)(_)  (__ _ __)(_)  (_) 
 *    (_)   (_)(_)___   ______ (_)__   (_)     (_)   (_)_(_)  
 *    (_)   (_)  (___)_(______)(____)  (_)     (_)   (____)   
 *     (_)_(_)   ____(_)       (_)     (_)____ (_)   (_) (_)  
 *      (___)   (_____)        (_)     (______)(_)   (_)  (_) 
 *                                                            
 *                                                            
 */

#include "utils/policies.hpp"
#include <iostream>
#include <filesystem>

#include <utils/env.hpp>
#include <loader.hpp>
#include <themes/default-cute.hpp>
#include <globals.hpp>

using namespace vs;

int run(const char* path, const char *entry, const char* profile, const char* tests=nullptr){
  globals::policy.debug();  //To set an initial record in the debug file.
  globals::policy.inherit(policies_t::from_env());
  globals::path_env = mk_env(path, entry);

  std::cout<<"\n--------- paths ---------\n";
  std::cout <<"cwd:  "<<globals::path_env.cwd.as_string()<<"\n"
            <<"app:  "<<globals::path_env.app_path.as_string()<<"\n"
            <<"vs:   "<<globals::path_env.root.as_string()<<"\n"
            <<"data: "<<globals::path_env.userdata_path.as_string()<<"\n"
            <<"repo: "<<globals::path_env.packages_path.as_string()<<"\n"
            <<"tmp:  "<<globals::path_env.tmp_path.as_string()<<"\n";


  try{
    std::filesystem::create_directories(globals::path_env.userdata_path.location);
    std::filesystem::create_directories(globals::path_env.packages_path.location);
    std::string db_path;
    {
      auto t = getenv("VS_DB");
      if(t!=nullptr)db_path=t;
      else db_path=globals::path_env.userdata_path.location+"db.sqlite";

      if(!std::filesystem::exists(db_path)){
        if(t==nullptr)std::filesystem::copy_file(globals::path_env.app_path.location+"commons/db.sqlite",db_path);
      }
    }
    app_loader loader(profile,entry);
  
    //TODO implement test
    auto t= loader.run();
    std::cout<<"\n";
    return t;
  }
  catch(const char* exception){
    std::cerr<<"Error: "<< exception<<"\n";
    return 1;
  }
  catch(const std::exception& exception){
    std::cerr<<"Error: "<< exception.what()<<"\n";
    return 1;
  }
}


int main(int argc, char **argv) {
  const char* profile = getenv("VS_PROFILE");

  if(argc==1){
    return run(argv[0],"vs://hub.xml",profile);       
  }
  else{
    if(strcmp(argv[1],"run")==0){
      if(argc<2){
        std::cerr<<"This application requires the path to a valid xml file or native component passed as first argument\n";
        return 1;
      }

      auto t =run(argv[0],argv[2],profile);
      return t;
    }
    else if(strcmp(argv[1],"test")==0){
      if(argc<2){
        std::cerr<<"This application in test mode requires the path to a valid xml file or native component as first argument, and an xml file of actions as its second\n";
        return 1;
      }

      auto t =run(argv[0],argv[2],profile,argv[3]);
      return t;
    }
    else if(strcmp(argv[1],"version")==0){
        versions_t versions = get_versions();

        std::cout<<"\n------- versions --------\n";
        std::cout <<"vs:       "<<versions.vs<<"\n"
                  <<"tcc:      "<<versions.tcc<<"\n"
                  <<"quickjs:  "<<versions.quickjs<<"\n"
                  <<"wamr:     "<<versions.wamr<<"\n"
                  <<"sqlite:   "<<versions.sqlite<<"\n"
                  <<"fltk:     "<<versions.fltk<<"\n"
                  <<"curl:     "<<versions.curl<<"\n"
                  <<"libuv:    "<<versions.libuv<<"\n";
        return 0;
    }
    else if(strcmp(argv[1],"editor")==0){
        return run(argv[0],"vs://editor.xml",profile);       
    }
    else if(strcmp(argv[1],"hub")==0){
        return run(argv[0],"vs://hub.xml",profile);       
    }
    else if(strcmp(argv[1],"help")==0){
      const char docs[] = {
        #embed "../../commons/HELP.md" suffix(, 0)      
      };
      //TODO: Show in window as a markdown once the processor is ready.
      std::cout<<docs<<"\n";
      return 0;    
    }
    else{
      std::cerr<<"Unrecognized option\n";
      std::cerr<<argv[1]<<"\n";
    }
  }

  
  //std::filesystem::remove_all(globals::path_env.tmp_path.location);
  return 0;
} 
