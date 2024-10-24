#include "FL/Fl.H"
#include "FL/Fl_Box.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Input.H"
#include "FL/Fl_Toggle_Button.H"
#include "FL/Fl_Widget.H"
#include "components/markdown-view.hpp"
#include "pipelines/quickjs-js.hpp"
#include "resolvers/fs.hpp"
#include "ui-frame.hpp"
#include "pipelines/tcc-c.hpp"
#include "utils/paths.hpp"
#include <cstdarg>

#include <ui.hpp>
#include <components/containers.hpp>
#include <ui-tree.xml.hpp>
#include <unistd.h>

#if __has_include("components/autogen/index.hpp")
#include <components/autogen/index.hpp>
#endif

//TODO: Add support for namespaces in macros
#define mkNodeWidget($ns,$name,$class_name) else if(strcmp(root.name(),#$name)==0){\
  auto t=build_base_widget<$class_name>(root,root_ui);\
  t->set_type(frame_type_t::NODE);\
  root_ui = tmp;\
  for(auto& i : root.children()){_build(i,root_ui);}\
  tmp->widget().end();\
} 
//TODO: Avoid parsing its head                                                 
#define mkLeafWidget($ns,$name,$class_name) else if(strcmp(root.name(),#$name)==0){build_base_widget<$class_name>(root,root_ui); }                                                  

namespace vs{

void ui_xml_tree::log(int severety, const void* _ctx, const char* str, ...){
    static const char* severity_table[] = {
    "\033[34;1m[INFO]\033[0m     : ",
    "\033[32;1m[OK]\033[0m       : ",
    "\033[33;1m[WARNING]\033[0m  : ",
    "\033[37;1m[CONTINUE]\033[0m : ",
    "\033[31;1m[PANIC]\033[0m    : ",
  };
    const pugi::xml_node& ctx = *(const pugi::xml_node*)_ctx;
    std::string rstr = std::string("\n")+std::string(severity_table[(int)severety%5]) + std::string(str) + " @ [\033[93;3m" + (_ctx!=nullptr?ctx.path():"???") + "\033[0m]";
    
    va_list args;
    va_start(args, str);
    vfprintf(log_device,rstr.c_str(), args);
    va_end(args);

    fflush(stdout);
  }


//General XML loader for apps and components.
//TODO: The caller node is a design flaw. We need to be given the list of props and slots. Not the full node which might not even exist.
int ui_xml_tree::load(const char* file, bool is_app, const pugi::xml_node* caller_node, ui_base* caller_ui_node, const scoped_rpath_t* caller_path)
{
  resolve_path resolver(policies,global_path_env,(caller_path==nullptr)?global_path_env.cwd:*caller_path);
  auto computed_path = resolver(resolve_path::from_t::NATIVE_CODE,file);
  if(computed_path.first!=resolve_path::reason_t::OK)return 2;
  this->local=computed_path.second.base_dir();
  this->fullname=computed_path.second;

  this->is_app = is_app;
  this->caller_node=caller_node;
  this->caller_ui_node=caller_ui_node;
  vs_log(severety_t::INFO, nullptr, "Requested loading of file `%s`", computed_path.second.as_string().data());

  //TODO: Move this to cache later on, but for now just dump the result into a buffer

  auto buffer = fs_fetch_to_new(computed_path.second,pugi::get_memory_allocation_function());
  pugi::xml_parse_result result =  doc.load_buffer_inplace_own(buffer.first, buffer.second); // doc.load_file(file);
  if (!result){
      return 1;
  }
  return 0;
}

ui_xml_tree::~ui_xml_tree(){if(root!=nullptr)delete root;}

int ui_xml_tree::build(){

  const auto& xml_root = doc.child(is_app?"app":"component");
  if(xml_root.empty()){
    log(severety_t::CONTINUE, xml_root, "Unable to find a valid root in `%s`", fullname.as_string().c_str());
    return 1;
  }


  ui_base* base;
  const auto& type = xml_root.attribute("type").as_string("native");
  if(strcmp(type,"native")==0){
    if(is_app){base = (ui_base*)new ui_root_app(frame_mode_t::VOID);}
    else base = caller_ui_node;//(ui_base*)new ui_root_component(frame_mode_t::DEFAULT);
  }
  else{
    log(severety_t::CONTINUE, xml_root, "An unsupported type `%s` was requested.", fullname.as_string().c_str());
    return 1;
  }

  _build(doc.child(is_app?"app":"component"),is_app?base:caller_ui_node);
  
  //base->widget().end();
  //base->widget().show();
  if(is_app)root=base;
  else root = nullptr;
  return 0;
}

void ui_xml_tree::_build(const pugi::xml_node& root, ui_base* root_ui){
    //std::cout<<root.name()<<root.text()<<root.value()<<'\n';
    //USE
    if(strcmp(root.name(),"use")==0){
      const auto& src =root.attribute("src");
      if(src.empty()){
        log(severety_t::CONTINUE,root,"<use/> must have a source");
      }
      else{
        log(severety_t::INFO,root,"Loading component %s", src);
        ui_xml_tree component_tree; 
        if(component_tree.load(src.as_string(),false,&root,root_ui,&local)!=0){
          log(severety_t::INFO,root,"Loading failed, file cannot be opened %s", src);
        }
        else{
          component_tree.build();
          nodes.insert(nodes.end(),component_tree.nodes.begin(),component_tree.nodes.end());
          component_tree.nodes.clear();
          return;
        }
      }

      //Collector of all failures
      const auto& error =root.child("on-load.fail");
      if(!error.empty()){
        _build(error,root_ui);
        return;
      }
      return;
    }
    //SLOT
    else if(strcmp(root.name(),"slot")==0){
      //Copy the content provided by the parent in place of the slot, or render its content if the parent has none.
      const auto& name = root.attribute("tag").as_string("default");
      //The default slot.
      if(this->caller_node!=nullptr){
        const auto& match = this->caller_node->find_child([&name](const pugi::xml_node& node){
          return (strcmp(node.name(),"inject")==0) && (strcmp(node.attribute("tag").as_string("default"),name)==0);
          });
          
        if(!match.empty()){
          for(auto& i : match.children()){_build(i,root_ui);}
          return;
        }
      }
      //Catch all slot not found.
      if(!root.attribute("required").empty()){
        log(severety_t::WARNING,root,"The `%s` slot was set as required but no override available.", name);
      }
      for(auto& i : root.children()){_build(i,root_ui);}
      
    }
    //WINDOW
    else if(strcmp(root.name(),"window")==0){
      auto tmp = build_base_widget<ui<Fl_Window>>(root,root_ui);
      tmp->set_type(frame_type_t::NODE);

      root_ui = tmp;
      for(auto& i : root.children()){_build(i,root_ui);}
      tmp->widget().end();
      tmp->widget().show();
      return;
    }
    //VIEWPORT
    else if(strcmp(root.name(),"viewport")==0){
      auto tmp = build_base_widget<ui_viewport>(root,root_ui);
      root_ui = tmp;
      for(auto& i : root.children()){_build(i,root_ui);}
      tmp->widget().end();

      return;
    }
    //GROUP
    else if(strcmp(root.name(),"group")==0){
      auto tmp = build_base_widget<ui<Fl_Group>>(root);
      tmp->set_access(frame_access_t::PUBLIC);
      root_ui = tmp;
      for(auto& i : root.children()){_build(i,root_ui);}
      tmp->widget().end();

      return;
    }
      //GROUP
    else if(strcmp(root.name(),"namespace")==0){
      auto tmp = build_base_widget<ui_namespace>(root);
      tmp->set_access(frame_access_t::PUBLIC);
      root_ui = tmp;
      for(auto& i : root.children()){_build(i,root_ui);}
      tmp->widget().end();

      return;
    }
    else if (strcmp(root.name(),"app")==0){

      _build_base_widget_extended_attr(root, (ui_base *)root_ui);
    
      //TODO: Optimize copies
      smap<std::string> props;

      for (const auto &i : root.attributes()) {
        props.insert_or_assign(i.name(), i.value());
      }

      for (const auto &i : props) {
        int v = root_ui->apply_prop(i.first.data(), i.second.data());
        if (v == 1) {
          log(severety_t::WARNING, root, "Unable to use property `%s` on ",
              i.first.data());
        } else if (v == 2) {
          log(severety_t::WARNING, root,
              "Unable to assign value `%s` to property `%s on", i.second.data(),
              i.first.data());
        }
      }

    }
    else if (strcmp(root.name(),"component")==0){/*skip*/}

    //Basic widgets
    mkLeafWidget(,button,ui<Fl_Button>)
    mkLeafWidget(,label,ui<Fl_Box>)
    mkLeafWidget(,input,ui<Fl_Input>)
    mkLeafWidget(,button.toggle,ui<Fl_Toggle_Button>)
    
#   if __has_include("./ui.xml-widgets.autogen.cpp")
#   include "./ui.xml-widgets.autogen.cpp"
#   endif


    else{return;}

    for(auto& i : root.children()){_build(i,root_ui);}
    return;
  }

  void ui_xml_tree::_build_base_widget_extended_attr(const pugi::xml_node &root, ui_base* current) {
    
    for (auto &root : root.children()) {

      // MIXIN
      if (strcmp(root.name(), "mixin") == 0) {
        smap<std::string> tmp;
        for (const auto &i : root.attributes()) {
          tmp.insert_or_assign(i.name(), i.value());
        }
        {auto it = tmp.find("name");if (it != tmp.end())tmp.erase(it);}
        {auto it = tmp.find("frame.type");if (it != tmp.end())tmp.erase(it);}
        {auto it = tmp.find("frame.access");if (it != tmp.end())tmp.erase(it);}
        {auto it = tmp.find("frame.mode");if (it != tmp.end())tmp.erase(it);}
        
        current->add_mixin(root.attribute("name").as_string(""), tmp);
        // Always remove the `name` attribute from mixins.
      }
      // SCRIPT.MODULE
      else if (strcmp(root.name(), "script.module") == 0) {
        current->mk_frame();
        if(current->get_local_frame()->has_script()){
          log(severety_t::WARNING, root, "Only one `script` or `script.module` is allowed per frame.");
          continue;
        }

        if(root.attribute("$cached").as_string()!=nullptr){
          //TODO: Recover the cached result
          continue;
        }
  
        if (mode == frame_mode_t::NATIVE || mode == frame_mode_t::VOID) {
          const auto &lang = root.attribute("lang").as_string(mode==frame_mode_t::NATIVE?"c":"");
          if (strcmp(lang, "c") == 0) {
            auto compiler = pipelines::tcc_c_pipeline_xml(true, nullptr, root, nullptr);
            if(compiler!=nullptr)current->attach_unique_script(compiler);
            current->set_mode(frame_mode_t::NATIVE);
            continue;
          }
        }
      }
      // SCRIPT
      else if (strcmp(root.name(), "script") == 0) {
        current->mk_frame();
        if(current->get_local_frame()->has_script()){
          log(severety_t::WARNING, root, "Only one `script` or `script.module` is allowed per frame.");
          continue;
        }
        //I am ignoring the one of the tree. Mode is now widget based and not component based.
        auto mode =current->get_local_frame()->get_mode();
        if (mode == frame_mode_t::NATIVE || mode == frame_mode_t::VOID) {
          const auto &lang = root.attribute("lang").as_string(mode==frame_mode_t::NATIVE?"c":"");
          if (strcmp(lang, "c") == 0) {
            //Maybe took it away from app and place it locally for each script? Not sure. 
            auto link_with = doc.first_child().attribute("link-with").as_string(nullptr);
            std::string tmp_link;
            if(link_with!=nullptr){
              resolve_path resolver(policies,global_path_env,local);
              auto computed_path = resolver(resolve_path::from_t::NATIVE_CODE,link_with);
              if(computed_path.first==resolve_path::reason_t::OK){
                //TODO: For now I am assuming it is on the fs. I should resolve it to tmp if remote for example
                tmp_link=computed_path.second.location;
                log(severety_t::INFO, root, "Requested linking with `%s`", link_with);
              }
            }
            auto compiler = pipelines::tcc_c_pipeline_xml(true, current, root, (link_with==nullptr)?nullptr:tmp_link.c_str());
            if(compiler!=nullptr){
              pipelines::tcc_c_pipeline_apply(compiler, current, (void*)&root, (void(*)(void*,const char*, const char*))pipelines::tcc_log_symbol_func_xml);
              current->attach_unique_script(compiler);
              current->set_mode(frame_mode_t::NATIVE);
            }
            continue;
          }
        }
        if (mode == frame_mode_t::QUICKJS || mode == frame_mode_t::VOID) {
          const auto &lang = root.attribute("lang").as_string(mode==frame_mode_t::QUICKJS?"js":"");
          if (strcmp(lang, "js") == 0) {
            auto compiler = pipelines::qjs_js_pipeline_single_xml(current, nullptr, root, true);
            if(compiler!=nullptr)current->attach_unique_script(compiler);
            current->set_mode(frame_mode_t::QUICKJS);
            continue;
          }
        }
        {
            log(severety_t::CONTINUE, root,
                "Unsupported language `%s` for frame type `%i`. The script will not be handled.",
                root.attribute("lang").as_string(), mode);
        }
      }
    }
  }


    template <std::derived_from<ui_base> T>
    T *ui_xml_tree::build_base_widget(const pugi::xml_node &root, ui_base* root_ui) {
      auto *current = new T(root_ui);
      nodes.push_back(current);

      {
        const auto& tmp = root.attribute("name");
        if (!tmp.empty()) {current->set_name(tmp.as_string());}
      }
      {
        const auto& tmp = root.attribute("frame.mode");
        if (!tmp.empty()) {
          if(strcmp(tmp.as_string(),"native")==0)current->set_mode(frame_mode_t::NATIVE);
          else if(strcmp(tmp.as_string(),"quickjs")==0)current->set_mode(frame_mode_t::QUICKJS);
          else{current->set_mode(frame_mode_t::VOID);}
        }
      }

      _build_base_widget_extended_attr(root, (ui_base *)current);
      
  
      if(strcmp(root.parent().name(),"app")==0){
        current->reparent_frame(root_ui);
      }

      //TODO: Optimize copies
      smap<std::string> props = current->compute_refresh_style(root.attribute("mixin").as_string(""));
      /*{auto tmp = current->compile_mixins((std::string("+")+root.name()).data()); for(const auto& i: tmp)props.insert_or_assign(i.first,std::move(i.second));}

       const auto &self_mixin = (root.find_child([](const auto &i) {
         return (strcmp(i.name(), "mixin") == 0) &&
                (strcmp(i.attribute("name").as_string(""), "") == 0);
       }));
       if (!self_mixin.empty()) {
         for (const auto &i : self_mixin.attributes()) {
           props.insert_or_assign(i.name(), i.value());
         }
         {
           auto it = props.find("name");
           if (it != props.end())
             props.erase(it);
         }
      }

      {auto tmp = current->compile_mixins(root.attribute("mixin").as_string("")); for(const auto& i: tmp)props.insert_or_assign(i.first,std::move(i.second));}
      */
      for (const auto &i : root.attributes()) {
        props.insert_or_assign(i.name(), i.value());
      }

      for (const auto &i : props) {
        int v = current->apply_prop(i.first.data(), i.second.data());
        if (v == 1) {
          log(severety_t::WARNING, root, "Unable to use property `%s` on ",
              i.first.data());
        } else if (v == 2) {
          log(severety_t::WARNING, root,
              "Unable to assign value `%s` to property `%s on", i.second.data(),
              i.first.data());
        }
      }
      return current;
    }
  }

  #undef mkNodeWidget
  #undef mkLeafWidget