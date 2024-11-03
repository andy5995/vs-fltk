#include <iostream>

#include <ui-tree.xml.hpp>
#include <loader.hpp>
#include <globals.hpp>

namespace vs{
app_loader::app_loader(const char *profile, const char* path){
    pugi::xml_document doc;
    
    if(profile!=nullptr){
      std::string profile_path = (globals::path_env.appdata_path.as_string()+profile+".xml");
      pugi::xml_parse_result result = doc.load_file(profile_path.c_str());
      if(!result){
        std::cout << "XML [" << profile_path << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
        std::cout << "Error description: " << result.description() << "\n";
        std::cout << "Error offset: " << result.offset <<"\n\n";
        std::cout << "Loading default profile instead\n\n";
      }
    }
    else{
      std::string profile_path = (globals::path_env.appdata_path.as_string()+"default.xml");
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


  root=new ui_xml_tree();
  if(root->load(path,true)!=0){
    throw "Unable to process file";
  }
  else{
    root->build();
    //vs::theme_cute::apply();
  }
}

int app_loader::run(){
  if(!globals::policy.headless){
    auto t= Fl::run();
    delete root;
    root=nullptr;
    return t;
  }
  else{
    //for(;;);
    return 0;
  }
}

    app_loader::~app_loader(){if(root!=nullptr)delete root;}

}