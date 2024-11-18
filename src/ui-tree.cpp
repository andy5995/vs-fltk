#include <cstring>
#include <ui-tree.hpp>

namespace vs{
  
  bool ui_tree::string2key256(const char* str, uint8_t array[256/32]){
    memset(array,0,256/8);
  }

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

  bool ui_tree::h_flag(bool* dst, const char* expr,  const ui_base* env){
    if(strcmp(expr, "false")==0){dst[0]=false;}
    else if(strcmp(expr,"true")==0){dst[0]=true;}
    return false;
  }

  ui_tree::~ui_tree(){
    for(auto i : nodes){
      delete i;
    }
    Fl::wait();
  }

}