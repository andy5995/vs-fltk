#include <iostream>
#include <ui-tree.hpp>

namespace vs{
  
  //TODO: I need to check if number conversions failed or not too.
  bool ui_tree::h_px(uint T, size_t* dst, const char* expr, const ui_base* env){
    if (T==1){
      dst[0]=std::stoi(expr);
      return true;
    }

    auto box = split(expr,',');
    if(box.size()!=T)return false;

    int c=0;
    for(const auto& i : box) dst[c++]=std::stoi(i);
    return true;
  }

  bool ui_tree::h_colour(uint32_t* dst, const char* expr,  const ui_base* env){
    if(expr[0]=='#'){
      dst[0] = std::stoi(expr+1,nullptr,16)<<8;
    }
    else{
      //TODO: named colours not supported yet. But indexed colors via index are.
      dst[0] = std::stoi(expr) & 0xff;
    }
    return true;
  }

  ui_tree::~ui_tree(){
    for(auto i : nodes){
      std::cout<<"Delete "<<i->get_name()<<"\n";
      delete i;
    }
    Fl::wait();
  }

}