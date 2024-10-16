#include <iostream>
#include <ostream>
#include <sstream>
#include <ui.hpp>
#include <ui-tree.hpp>


#include "FL/Enumerations.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Group.H"
#include "FL/Fl_Input.H"

#include "FL/Fl_Toggle_Button.H"
#include "FL/Fl_Widget.H"
#include "ui-frame.hpp"
#include "utils/strings.hpp"

namespace vs{

inline void ui_base::mk_frame(const char* name, frame_mode_t mode){
  if(local_frame==nullptr){
    auto tmp = (frame*)resolve_namespace();
    local_frame=new frame(name, mode , this, tmp, default_frame_type(), frame_access_t::PRIVATE);
  }
}

void ui_base::set_name(const char* name){
  mk_frame();
  local_frame->rename(name);
}

void ui_base::set_access(frame_access_t access){
  mk_frame();
  local_frame->access= access;
}

void ui_base::set_type(frame_type_t type){
  mk_frame();
  local_frame->type = type;
}

void ui_base::set_mode(frame_mode_t mode){
  mk_frame();
  local_frame->mode = mode;
}

const std::string& ui_base::get_name() const{
  static std::string defname = "";
  if(local_frame==nullptr)return defname;
  else{
    return local_frame->name;
  }
}

void ui_base::path(std::stringstream& dst, bool scoped)const{
    frame* current = this->local_frame;
    while(current!=nullptr){
        dst<<current->name<<"/";
        if(scoped==true && (current->type==frame_type_t::CONTAINER or current->type==frame_type_t::SLOT_CONTAINER))break;
        current=current->parent;
    }
    if(scoped)dst<<"#";
    else dst<<"@";
}

ui_base::~ui_base(){
  if(local_frame!=nullptr){
    delete local_frame;
    local_frame=nullptr;
  }
}


frame* ui_base::resolve_frame() const{
  Fl_Widget* p = this->widget().parent();
  while(p!=nullptr){
    ui_base& parent = *FL_TO_UI(*(Fl_Widget*)p); //TODO: Clean up this nasty type chain at some point plz.
    if(parent.local_frame!=nullptr)return parent.local_frame;
    p=p->parent();
  }
  return nullptr;
}

frame* ui_base::resolve_namespace() const{
  const Fl_Widget* p = this->widget().parent();
  while(p!=nullptr){
    ui_base& parent = *FL_TO_UI(*(Fl_Widget*)p); //TODO: Clean up this nasty type chain at some point plz.
    if(parent.local_frame!=nullptr && (parent.local_frame->get_type()==frame_type_t::CONTAINER  || parent.local_frame->get_type()==frame_type_t::NODE  ||  parent.local_frame->get_type()==frame_type_t::SLOT_CONTAINER))return parent.local_frame;
    //Top level windows are not attached to app via FL
    else if(parent.local_frame!=nullptr && parent.local_frame->parent!=nullptr)p=&(parent.local_frame->parent->widget()->widget());
    else p=p->parent();
  }
  return nullptr;
}

const ui_base* ui_base::resolve_name_path(const char * str) const{
  static char buffer[64];
  const ui_base* current = this;
  for(int i=0;str[i]!=0;){
    int j=i;
    for(;str[j]!='/' && str[j]!=0 && (j-i <64);j++)buffer[j-i]=str[j];    //TODO exception if going beyond the buffer limits?
    buffer[j-i]=0;
    current=current->resolve_name(buffer);
    i=j;
    if(str[i]!=0)i++;
    if(current==nullptr)return nullptr;
  }
  return current;
}

const ui_base* ui_base::resolve_name(const char * str) const{
  if(local_frame!=nullptr){
    auto tmp = local_frame->resolve_name(str);
    if(tmp==nullptr)return nullptr;
    else return tmp->widget(); 
  }
  else{

    auto frame =  this->resolve_frame();

    if(frame!=nullptr){
      auto tmp = frame->resolve_name(str);
      if(tmp==nullptr)return nullptr;
      else return tmp->widget(); 
    }
    else return nullptr;
  }
}

 symbol_ret_t ui_base::resolve_symbol(const char * str) const{
  if(local_frame!=nullptr){
    return local_frame->resolve_symbol(str);
  }
  else{

    auto frame =  this->resolve_frame();

    if(frame!=nullptr){return frame->resolve_symbol(str);}
    else return {symbol_t::VOID, symbol_t::VOID, nullptr};
  }
}

void ui_base::set_dispatcher(symbol_t value){
    mk_frame();
    local_frame->custom_dispatcher = value;
}

void ui_base::register_symbol(const char* name, symbol_t value){
    mk_frame();
    return local_frame->register_symbol(name, value);
}

symbol_t ui_base::get_symbol(const char* name){
    mk_frame();
    return local_frame->get_symbol(name);
}

void ui_base::unregister_symbol(const char* name){
    mk_frame();
    return local_frame->unregister_symbol(name);
}

void ui_base::reset_symbols(){
    mk_frame();
    return local_frame->reset_symbols();
}

void ui_base::attach_unique_script(const std::shared_ptr<void>& ref){
    mk_frame();
    local_frame->context.unique = ref;
}

void ui_base::attach_shared_script(const std::shared_ptr<void>& ref){
    mk_frame();
    local_frame->context.shared = ref;
}

void ui_base::add_mixin(const char* name, const smap<std::string>& map){
    mk_frame();
    local_frame->mixins.emplace(name,std::move(map));
}

//Resolve a mixin based on its name
std::pair<const smap<std::string>*,const frame*> ui_base::resolve_mixin(const char* name) const{
  if(local_frame!=nullptr){
    return local_frame->resolve_mixin(name);
  } else {

    auto frame = this->resolve_frame();

    if (frame != nullptr) {
      return frame->resolve_mixin(name);
    } else
      return {nullptr,local_frame};
  }
}



//Compile a single prop based on a mixin definition
const char* ui_base::eval_prop(const char* prop, const char* mixins[]) const{
  //TODO
  return {};
}


smap<std::string> ui_base::compile_mixins(const char* list_mixins) const {
  if(local_frame!=nullptr){
    return local_frame->compile_mixins(list_mixins);
  } else {

    auto frame = this->resolve_frame();

    if (frame != nullptr) {
      return frame->compile_mixins(list_mixins);
    } else
      return {};
  }
}


smap<std::string> ui_base::compute_refresh_style(const char* local_mixins){
      //TODO: Optimize copies
      smap<std::string> props = this->compile_mixins("*");
      {auto tmp = this->compile_mixins((std::string("+")+this->class_name()).data()); for(const auto& i: tmp)props.insert_or_assign(i.first,std::move(i.second));}

      if(this->local_frame!=nullptr){
        auto self_mixin = this->local_frame->resolve_mixin("");

        if (self_mixin.first == nullptr){}
        else {
          //Take and apply its own mixins first!
          auto key = self_mixin.first->find("mixin");
          if(key!=self_mixin.first->end() && self_mixin.second!=nullptr){
            //Search for elements to mix in STARTING from the frame this was obtained, and not from the original caller. Otherwise inheritance is broken.
            auto tmp2 = self_mixin.second->compile_mixins(key->second.data());
            for(const auto& i: tmp2)props.insert_or_assign(i.first,std::move(i.second));
          }

          for (const auto &j : *self_mixin.first) {
            props.insert_or_assign(j.first, j.second);
          }
        }
      }

      {auto tmp = this->compile_mixins(local_mixins); for(const auto& i: tmp)props.insert_or_assign(i.first,std::move(i.second));}

      return props;
}

void ui_base::refresh_style(const char* local_mixins){
  smap<std::string> props = compute_refresh_style(local_mixins);
  for (const auto &i : props) {
    int v = this->apply_prop(i.first.data(), i.second.data());
    if (v == 1) {
      vs_log(severety_t::WARNING | severety_t::LVL_DEBUG , this, "Unable to use property `%s` on ",
          i.first.data());
    } else if (v == 2) {
      vs_log(severety_t::WARNING, this,
          "Unable to assign value `%s` to property `%s on", i.second.data(),
          i.first.data());
    }
  }
}




}