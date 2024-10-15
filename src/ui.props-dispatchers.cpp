// All code for property props goes here.
// This only covers the writable interface to be exposed via xml props & c bindings
#include <FL/Enumerations.H>
#include <ui-tree.hpp>
#include <ui.hpp>
#include <ui-frame.hpp>

#include <pipelines/quickjs-js.hpp>


#include "FL/Enumerations.H"

#include "FL/Fl_Box.H"
#include "FL/Fl_Widget.H"
#include "FL/Fl_Group.H"
#include "FL/Fl_Window.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Toggle_Button.H"
#include "FL/Fl_Input.H"

#include <components/containers.hpp>
#include <components/markdown.hpp>


namespace vs{


template<> const char* ui<Fl_Button>::class_name()  {return "button";}
template<> const char* ui<Fl_Toggle_Button>::class_name()  {return "button.toggle";}
template<> const char* ui<Fl_Input>::class_name()  {return "input";}
template<> const char* ui<Fl_Box>::class_name()  {return "label";}
template<> const char* ui<Fl_Markdown>::class_name()  {return "markdown";}

#if __has_include("./ui.names.autogen.cpp")
#include "./ui.names.autogen.cpp"
#endif

//TODO: this can become the generic approach to handle symbols based on their type. For now just callbacks from the ui
struct ui_callback_t : Fl_Callback_User_Data{
  enum {FL_CALLBACK, SETTER, GETTER} type;
  ui_base* caller;
  symbol_ret_t sym;
};

//TODO: Refresh logic to check callback type
void ui_callback_handler(Fl_Widget* _, void* _data){
  ui_callback_t* data = (ui_callback_t*)_data;
  void (*fn)(ui_base*)=(void (*)(ui_base*))data->sym.symbol.symbol;
  if(data->sym.found_at->get_mode()==frame_mode_t::NATIVE){
    if(data->sym.ctx_apply.symbol!=nullptr){
      const ui_base* (*ctx_apply)(const ui_base*) = ( const ui_base* (*)(const ui_base*) ) data->sym.ctx_apply.symbol;
      const ui_base* tmp =ctx_apply(data->sym.found_at->widget());
      fn(data->caller);
      ctx_apply(tmp);
    }
    else fn(data->caller);
  }
  else if(data->sym.found_at->get_mode()==frame_mode_t::QUICKJS){
    bindings::quickjs_t* ctx = (bindings::quickjs_t*)data->sym.found_at->get_context().unique.get();
    auto globalThis = JS_GetGlobalObject(ctx->ctx);
    auto ret= JS_Call(ctx->ctx,ctx->handles[(size_t)data->sym.symbol.symbol-1],globalThis,0,nullptr);
    JS_FreeValue(ctx->ctx, ret);
    JS_FreeValue(ctx->ctx, globalThis);
  }
  else{
    //Callback type not supported yet.
  }
}

#define $eq(b) else if(strcmp((prop),(b))==0)
#define $start_prop($class_name) template<> int ui<$class_name>::_apply_prop(ui* that, const char* prop, const char* value){\
  auto& w = that->widget();\
  bool ok = true;
#define $end_prop($base_name) if(!ok) return 2; else return ui<$base_name>::_apply_prop((ui<$base_name>*)that,prop,value);}
#define $end_prop_b() else{ return 2; } return ok?0:1; }

#define $start_computed($class_name) template<> int ui<$class_name>::_get_computed(ui* that, const char* prop, const char** value){\
  auto& w = that->widget();\
  bool ok = true;
#define $end_computed($base_name) if(!ok) return 2; else return ui<$base_name>::_get_computed((ui<$base_name>*)that,prop,value);}
#define $end_computed_b() else{ return 2; } return ok?0:1; }

$start_prop(Fl_Widget){
    if(false){}
    //Global exclusions, to be handled by something else.
    $eq("name"){}
    $eq("frame.type"){} $eq("frame.access"){} $eq("frame.mode"){} 
    $eq("xsml"){} else if(strncmp((prop),"xsml:",5)==0) {}  //Skip xml namespaces as I am not able to parse them for real.
    //To handle
    $eq("mixin"){that->refresh_style(value);} 
    $eq("label"){w.copy_label(value);}
    $eq("label.txt"){w.copy_label(value);}
    /*$eq("label.align"){
        //TODO: Add subfields and masks. It must be able to OR multiple values
        //IMPLEMENT AS A SINGLE INSTRUCTION
        //w.align(ui_tree::fl_align_i(value));
    }*/
    $eq("label.align.pos"){
        int t  = ui_tree::fl_align_pos_i(value);
        if((ok=(t!=-1)))w.align((w.align()&(!FL_ALIGN_POSITION_MASK))|t);
    }
    $eq("label.align.img"){
        int t  = ui_tree::fl_align_image_i(value);
        if((ok=(t!=-1)))w.align((w.align()&(!FL_ALIGN_IMAGE_MASK))|t);
    }
    $eq("label.align.wrap"){
        int t  = ui_tree::fl_align_wrap_i(value);
        if((ok=(t!=-1)))w.align((w.align()&(!FL_ALIGN_WRAP))|t);
    }
    $eq("label.align.clip"){
        int t  = ui_tree::fl_align_clip_i(value);
        if((ok=(t!=-1)))w.align((w.align()&(!FL_ALIGN_CLIP))|t);
    }
    $eq("label.align.inside"){
        int t  = ui_tree::fl_align_inside_i(value);
        if((ok=(t!=-1)))w.align((w.align()&(!FL_ALIGN_INSIDE))|t);
    }
    $eq("label.size"){
        if(strcmp(value,"default")==0){w.labelsize(FL_NORMAL_SIZE);}
        else{
        size_t size; if((ok=ui_tree::h_px(1,&size,value,that))) w.labelsize(size);
        }
    }
    $eq("label.colour"){uint32_t c; if((ok=ui_tree::h_colour(&c,value,that)))w.labelcolor(c);}
    $eq("label.margin"){
        size_t box[2];
        if((ok = ui_tree::h_px(2,box,value,that))){
        w.horizontal_label_margin(box[0]);
        w.vertical_label_margin(box[1]);
        }
    }
    $eq("label.margin.x"){
        size_t box;
        if((ok = ui_tree::h_px(1,&box,value,that))){
        w.horizontal_label_margin(box);
        }
    }
    $eq("label.margin.y"){
        size_t box;
        if((ok = ui_tree::h_px(1,&box,value,that))){
        w.vertical_label_margin(box);
        }
    }
    //...

    $eq("box"){
        size_t box[4];
        if((ok = ui_tree::h_px(4,box,value,that))){
        w.position(box[0],box[1]);
        w.size(box[2],box[3]);
        }
    }
    $eq("box.x"){size_t v; if((ok=ui_tree::h_px(1,&v,value,that))) w.position(v, w.y());}
    $eq("box.y"){size_t v; if((ok=ui_tree::h_px(1,&v,value,that))) w.position(w.x(), v);}
    $eq("box.w"){size_t v; if((ok=ui_tree::h_px(1,&v,value,that))) w.size(v, w.h());}
    $eq("box.h"){size_t v; if((ok=ui_tree::h_px(1,&v,value,that))) w.size(w.w(), v);}
    $eq("box.style"){w.box(ui_tree::fl_boxtype_i(value));}
    $eq("bg.colour"){uint32_t c; if((ok=ui_tree::h_colour(&c,value,that))){w.color(c);w.selection_color(c);}}
    $eq("bg.colour.relaxed"){uint32_t c; if((ok=ui_tree::h_colour(&c,value,that)))w.color(c);}
    $eq("bg.colour.selected"){uint32_t c; if((ok=ui_tree::h_colour(&c,value,that)))w.selection_color(c);}

    $eq("on.callback"){ 
        //ASSUMING SYMBOLS ARE STATICALLY DEFINED
        if(strcmp(value,"")==0)w.callback((Fl_Callback*)nullptr);
        else{
        auto result = that->resolve_symbol(value);
        if(result.symbol.symbol==nullptr){
            vs_log(severety_t::CONTINUE,that,"Unable to find symbol `%s` for callback.",value);
        }
        else{
          ui_callback_t* payload = new ui_callback_t();
          payload->type=ui_callback_t::FL_CALLBACK;
          payload->caller=that;
          payload->sym=result;
          w.callback(ui_callback_handler,payload,true);
        }
        }
    }

    $eq("tooltip"){w.copy_tooltip(value);}

    $end_prop_b();
}
  /*
    const auto& attr_label_img_active = obj.attribute("label.img.selected");
    const auto& attr_label_img_inactive = obj.attribute("label.img.relaxed");
    const auto& attr_label_img_spacing = obj.attribute("label.img.spacing");

    const auto& attr_label_font = obj.attribute("label.font");
    const auto& attr_label_type = obj.attribute("label.type");

    const auto& attr_shortcut = obj.attribute("shortcut");

    const auto& attr_enable = obj.attribute("enable");
    const auto& attr_show = obj.attribute("show");
    const auto& attr_needKeyboard = obj.attribute("need-keyboard");
  */

$start_prop(Fl_Button){
  if(false){}
  //To handle
  $eq("test"){w.copy_label("TESTO!");}

  $end_prop(Fl_Widget);
}

$start_prop(Fl_Window){
  $end_prop(Fl_Widget);
}

$start_prop(Fl_Group){
  $end_prop(Fl_Widget);
}

$start_prop(Fl_Toggle_Button){
  $end_prop(Fl_Button);
}

$start_prop(Fl_Input){
  $end_prop(Fl_Button);
}

$start_prop(Fl_Box){
  $end_prop(Fl_Widget);
}

$start_prop(Fl_Markdown){
  $end_prop(Fl_Widget);
}


int ui_viewport::_apply_prop(ui_viewport* that, const char* prop, const char* value){
  auto& w = that->widget();
  bool ok = true;
  if(false){}
  $eq("src"){that->src=value;that->refresh_content();}
  if(!ok) return 2; 
  else return ui<Fl_Window>::_apply_prop((ui<Fl_Window>*)that,prop,value);
}



#if __has_include("./ui.apply_prop.autogen.cpp")
#include "./ui.apply_prop.autogen.cpp"
#endif


////////////////////////////////////////////

$start_computed(Fl_Widget){
    if(false){}
    $end_computed_b();
}

$start_computed(Fl_Button){
  $end_computed(Fl_Widget);
}

$start_computed(Fl_Window){
  $end_computed(Fl_Widget);
}

$start_computed(Fl_Group){
  $end_computed(Fl_Widget);
}

$start_computed(Fl_Toggle_Button){
  $end_computed(Fl_Button);
}

$start_computed(Fl_Input){
  $end_computed(Fl_Button);
}

$start_computed(Fl_Box){
  $end_computed(Fl_Widget);
}

$start_computed(Fl_Markdown){
  $end_computed(Fl_Widget);
}

#if __has_include("./ui.get_computed.autogen.cpp")
#include "./ui.get_computed.autogen.cpp"
#endif

#undef $eq
#undef $start_prop
#undef $end_prop
#undef $end_prop_b
#undef $start_computed
#undef $end_computed
#undef $end_computed_b

}
