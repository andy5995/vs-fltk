#include <cstdarg>
#include <ostream>
#include <sstream>
#include <ui.hpp>
#include <ui-tree.hpp>

#include "ui-frame.hpp"

namespace vs{


void vs_log(severety_t severety, const ui_base* ctx, const char* str, ...){
  static const char* sevtable[] = {
    "\033[34;1m[INFO]\033[0m     : ",
    "\033[32;1m[OK]\033[0m       : ",
    "\033[33;1m[WARNING]\033[0m  : ",
    "\033[37;1m[CONTINUE]\033[0m : ",
    "\033[31;1m[PANIC]\033[0m    : ",
    "\033[47;30;1m[LOG]\033[0m      : ",

  };
  
  std::string rstr = std::string("\n")+std::string(sevtable[(int)severety%6]) + std::string(str);  
  va_list args;
  va_start(args, str);
  vprintf(rstr.c_str(), args);
  va_end(args);

  std::stringstream name;
  name<<"";
  if(ctx!=nullptr){
    ctx->path(name);
    printf(" @ [\033[93;3m%s\033[0m]",name.str().c_str());
  }

  fflush(stdout);
}



}