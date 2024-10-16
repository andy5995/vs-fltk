#pragma once

#include "ui-frame.hpp"
#include <FL/Fl_Group.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>

#include <ui.hpp>

namespace vs{


class ui_root_app :public ui_base{
  protected:
    //Hack to ensure widget()->parent() does not fail. Not super safe in C++, but good enough for me.
    void* fake_parent = nullptr;

  public:
    ui_root_app(frame_mode_t MODE):ui_base(){
      //Cannot use mk_frame as it requires recursion and the widget property to operate.
      local_frame=new frame("%root", MODE, this, nullptr, default_frame_type(), frame_access_t::PUBLIC);
  }

  virtual frame_type_t default_frame_type() override {return frame_type_t::CONTAINER;}
  virtual const char* class_name() override{return "root";}

  int apply_prop(const char *prop, const char *value) override{if(local_frame!=nullptr)return local_frame->call_dispatcher(prop,value);return 1;}
  int get_computed(const char *prop, const char **value) override{return 1;}
};

class ui_root_component :public ui_base{
  protected:
    //Hack to ensure widget()->parent() does not fail. Not super safe in C++, but good enough for me.
    void* fake_parent = nullptr;


  public:
    ui_root_component(frame_mode_t MODE):ui_base(){
        //Cannot use mk_frame as it requires recursion and the widget property to operate.
        local_frame=new frame("%component", MODE, this, nullptr, default_frame_type(), frame_access_t::PUBLIC);
    }

    virtual frame_type_t default_frame_type() override {return frame_type_t::CONTAINER;}
    virtual const char* class_name() override{return "component";}

    int apply_prop(const char *prop, const char *value) override{if(local_frame!=nullptr)return local_frame->call_dispatcher(prop,value);return 1;}
    int get_computed(const char *prop, const char **value) override{return 1;}

};

class ui_namespace : public ui<Fl_Group>{
  public:
    ui_namespace():ui<Fl_Group>(0, 0, 0, 0){
      this->widget().size(widget().parent()->w(), widget().parent()->h());
      this->mk_frame();
      this->set_access(frame_access_t::PUBLIC);
  }

  virtual frame_type_t default_frame_type() override {return frame_type_t::NODE;}
  virtual const char* class_name() override{return "namespace";}

};

class ui_viewport : public ui<Fl_Window>{
  protected:
    std::string src;
    enum status_t{LOADING, READY, FAILED} status;
    ui_base* content_loading;
    ui_base* content;
    ui_base* content_fail;

  public:
    template<typename... Args>
    ui_viewport(Args ...w):ui<Fl_Window>(w...){mk_frame();}
    virtual frame_type_t default_frame_type() override {return frame_type_t::SLOT_CONTAINER;}
    virtual const char* class_name() override{return "viewport";}

    void refresh_content();

    static int _apply_prop(ui_viewport* that, const char* prop, const char* value);
    static int _get_computed(ui_viewport* that, const char* prop, const char** value);
};

}