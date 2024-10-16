#include "ui-tree.xml.hpp"
#include "utils/env.hpp"
#include <loader.hpp>

namespace vs{
app_loader::app_loader(const char* path){
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
  if(!global_policy.headless){
    auto t= Fl::run();
    delete root;
    root=nullptr;
    return t;
  }
  else return 0;
}

    app_loader::~app_loader(){if(root!=nullptr)delete root;}

}