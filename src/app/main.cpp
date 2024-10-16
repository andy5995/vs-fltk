
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

#include <iostream>
#include <filesystem>

#include <utils/env.hpp>
#include <loader.hpp>
#include <themes/default-cute.hpp>

#include <pugixml.hpp>

using namespace vs;


int run(const char* path, const char *entry, const char* profile){
   global_policy.inherit_from_env();
   global_path_env = mk_env(path, entry);

        std::cout<<"\n--------- paths ---------\n";
        std::cout <<"cwd:      "<<global_path_env.cwd.as_string()<<"\n"
                  <<"app:      "<<global_path_env.app_path.as_string()<<"\n"
                  <<"root:     "<<global_path_env.root.as_string()<<"\n"
                  <<"appdata:  "<<global_path_env.appdata_path.as_string()<<"\n"
                  <<"packages: "<<global_path_env.packages_path.as_string()<<"\n"
                  <<"tmp:      "<<global_path_env.tmp_path.as_string()<<"\n";

        std::filesystem::create_directories(global_path_env.appdata_path.location);
        std::filesystem::create_directories(global_path_env.packages_path.location);
        //std::filesystem::create_directories(global_path_env.tmp_path.as_string()); TODO: enable once it has unique suffix

        pugi::xml_document doc;
        
        if(profile!=nullptr){
          std::string profile_path = (global_path_env.appdata_path.as_string()+profile+".xml");
          pugi::xml_parse_result result = doc.load_file(profile_path.c_str());
          if(!result){
            std::cout << "XML [" << profile_path << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
            std::cout << "Error description: " << result.description() << "\n";
            std::cout << "Error offset: " << result.offset <<"\n\n";
            std::cout << "Loading default profile instead\n\n";
          }
        }
        else{
          std::string profile_path = (global_path_env.appdata_path.as_string()+"default.xml");
          pugi::xml_parse_result result = doc.load_file(profile_path.c_str());
          if(!result){
            std::cout << "XML [" << profile_path << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
            std::cout << "Error description: " << result.description() << "\n";
            std::cout << "Error offset: " << result.offset <<"\n\n";
            std::cout << "Loading embedded profile instead\n\n";
          }
          else{
            doc.load_string("");
          }
        }
    
    

        try{
            app_loader loader(entry);
            auto t= loader.run();
            std::cout<<"\n";
            return t;
        }
        catch(const char* exception){
          std::cerr<<"Error"<< exception<<"\n";
          return 1;
        }

}

int main(int argc, char **argv) {
  if(argc==1){
    return run(argv[0],"./commons/hub.xml",nullptr);       
  }
  else{
    if(strcmp(argv[1],"run")==0){
      if(argc<2){
        std::cerr<<"This application requires the path to a valid xml file or native component passed as first argument\n";
        return 1;
      }

      return run(argv[0],argv[2],nullptr);
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
    else if(strcmp(argv[1],"run-profile")==0){
        if(argc<3){
          std::cerr<<"This application requires the path to a profile and a valid xml file or native component passed as first argument\n";
          return 1;
        }
        
        return run(argv[0],argv[3],argv[2]);       
    }
    else if(strcmp(argv[1],"editor")==0){
        return run(argv[0],"./commons/editor.xml",nullptr);       
    }
    else if(strcmp(argv[1],"hub")==0){
        return run(argv[0],"./commons/hub.xml",nullptr);       
    }
    else if(strcmp(argv[1],"help")==0){
        std::cerr<<"Not implemented\n";
        return 0;    
    }
    else{
      std::cerr<<"Unrecognized option\n";
    }
  }

  
  //std::filesystem::remove_all(global_path_env.tmp_path.location);
 
  return 0;
} 
