#include <ui-frame.private.hpp>

namespace vs{

 
//Generate a mixin map based on a list of mixins
smap<std::string> frame::compile_mixins(const char* list_mixins) const {
  smap<std::string> tmp;

  char *buffer = new char [strlen(list_mixins)+1];
  strcpy(buffer,list_mixins);
  int i = 0, j = 0;
  bool last = false;
   while (true) {
    for (; buffer[i] != ',' && buffer[i]!=0;i++){}

    if(buffer[i]==0)last = true;
    else buffer[i]=0;

    auto current = resolve_mixin(buffer+j);
      


    if (current.first == nullptr){}
    else {
      //Take and apply its own mixins first!
      auto key = current.first->find("mixin");
      if(key!=current.first->end() && current.second!=nullptr){
        //Search for elements to mix in STARTING from the frame this was obtained, and not from the original caller. Otherwise inheritance is broken.
        auto tmp2 = current.second->compile_mixins(key->second.data());
        for(const auto& i: tmp2)tmp.insert_or_assign(i.first,std::move(i.second));
      }

      for (const auto &j : *current.first) {
        tmp.insert_or_assign(j.first, j.second);
      }
    }

    i++;
    j=i;
    if(last)break;
  }

  delete[] buffer;

  return tmp;
}

int frame::call_dispatcher(const char* key, const char* value){
  if(custom_dispatcher.symbol!=nullptr){
    if(custom_dispatcher.mode==symbol_mode_t::NATIVE){
        //Even in single script it wants a reference to the caller.
        int(*fn)(const ui_base*, const char* k, const char *v) = (  int(*)(const ui_base*, const char* k, const char *v)) custom_dispatcher.symbol;
        return fn(this->widget(),key,value);
    }
    else if(custom_dispatcher.mode==symbol_mode_t::QUICKJS){
      //TODO: Implement custom dispatching properly
      return 1;
    }
    else return 1;
  }
  else return 1;
}

symbol_t symbol_t::VOID= {symbol_mode_t::AUTO,symbol_type_t::VOID,nullptr};


}