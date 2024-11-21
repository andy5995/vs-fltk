#pragma once 

#include <memory>

#include <FL/Fl_Widget.H>

#include <string>
#include <utils/strings.hpp>
#include <ui-frame.hpp>

namespace vs{

class frame{
  template<std::derived_from<Fl_Widget> T, int SubType>
  friend class ui;

  friend class ui_base;

  public:
    struct message_t{

    };

    typedef bool(*filter_t)(message_t*);
    typedef bool(*resolver_t)(message_t*);

  protected:
    std::shared_ptr<void> script;
    std::shared_ptr<smap<symbol_t>> symbols = nullptr;  //For symbol resolution on function calling 
    
    frame_mode_t mode = frame_mode_t::AUTO;
    frame_type_t type;
    frame_access_t access;

    bool is_script_module = false;

    std::string name;
    frame* parent = nullptr;
    ui_base* ui_node =nullptr;

    smap<frame*> children;
    symbol_t custom_dispatcher = symbol_t::VOID;
    //smap<filter_t> filters;                           //To prevent top/down propagation of messages
    //smap<resolver_t> resolvers;                       //To catch messages coming from bottom up

    smap<smap<std::string>> mixins;   //Provide css-like classes for itself and children based on the same scoping rules for symbols.


    //Propagate search of a name down to children
    frame* _resolve_name_down(const char* name, const frame* origin) const{
      auto t = children.find(name);
      if(t!=children.end()){return t->second;}

      //Failure? Try to check inside in case children are passthrough.
      for(const auto& i : children){
        if(type==frame_type_t::NODE || type==frame_type_t::CONTAINER){
            auto w = i.second->_resolve_name_down(name, origin);
            if(w!=nullptr) return w;
        }
      }
      return nullptr;
    }

    const frame* _resolve_name(const char* name, const frame* origin) const {
      if(name[0]==0)return nullptr;

      //Self      
      if(strcmp(name,".")==0){
        return this;
      }
      //Closest reachable namespace parent
      else if(strcmp(name,"..")==0){
        if(type==frame_type_t::CONTAINER || type==frame_type_t::SLOT_CONTAINER || parent == nullptr)return nullptr;
        if(parent->name==""){return parent->_resolve_name("..", parent);}
        else{
          return parent;
        }
      }
      //Furthest reachable parent
      else if(strcmp(name,"...")==0){
        if(type==frame_type_t::CONTAINER || type==frame_type_t::SLOT_CONTAINER || parent == nullptr)return nullptr;

        const frame* saved = nullptr;
        for(const frame* current=parent;current!=nullptr;current=current->_resolve_name("..", current))saved=current;
        return saved;
      }

      //TODO: Evaluate if I want this rule, but I think not.
      //else if((type==frame_type_t::LEAF) && parent!=nullptr)return parent->_resolve_name(name, this);

      //Else look for the name!
      return _resolve_name_down(name,origin);
    }

    //Symbols are forced to match the tree structure to be found, from bottom to top
    symbol_ret_t _resolve_symbol(const char* name, const frame* origin) const{
      //No one else can see my stuff!
      if(access==frame_access_t::PRIVATE && origin!=this){
        if(parent!=nullptr){
          return parent->_resolve_symbol(name, origin);
        }
        return {
           symbol_t::VOID,
           symbol_t::VOID,
           nullptr
        };
      }

      //If not private
      if(symbols!=nullptr){
        auto t = symbols->find(name);
        if(t!=symbols->end() && !is_script_module){return {t->second,symbol_t::VOID,this};}
        else if(t!=symbols->end() && is_script_module){return {t->second,symbols->find("vs_set_env")->second,this};}
      } 

      //If I am a container I cannot let this progress any further.
      if(type==frame_type_t::CONTAINER or type==frame_type_t::SLOT_CONTAINER){
        return symbol_ret_t{
          {symbol_mode_t::AUTO,symbol_type_t::VOID,nullptr},
          {symbol_mode_t::AUTO,symbol_type_t::VOID,nullptr},
           nullptr
        };
      } else if (parent != nullptr) {
        return parent->_resolve_symbol(name, origin);
      }
        return symbol_ret_t{
          {symbol_mode_t::AUTO,symbol_type_t::VOID,nullptr},
          {symbol_mode_t::AUTO,symbol_type_t::VOID,nullptr},
           nullptr
        };
    }

    std::pair<const smap<std::string>*,const frame*>_resolve_mixin(const char* name, const frame* origin) const{
      auto t = mixins.find(name);
      if (t != mixins.end()) {
        return {&t->second,this};
      }
      // If I am a container I cannot let this progress any further.
      if (type == frame_type_t::CONTAINER or
          type == frame_type_t::SLOT_CONTAINER) {
        return {nullptr, this};
      } else if (parent != nullptr && strcmp(name,"")!=0) { //Skip parent search for self mixin.
        return parent->_resolve_mixin(name, origin);
      }
      return {nullptr, this};
    }

  public:
    inline frame_type_t get_type() const {return type;}
    inline frame_access_t get_access() const {return access;}
    inline frame_mode_t get_mode() const {return mode;}

    inline bool has_script() const {return script!=nullptr;}

    int call_dispatcher(const char* key, const char* value);
    int call_on_test();

    frame(const char* name, frame_mode_t mode, ui_base* ui_node, frame* parent, frame_type_t type, frame_access_t access){
      static int counter = 0;
      if(name==nullptr)this->name=std::string("%__")+std::to_string(counter++); //TODO: replace with hash to make them not predictable. Filter out %__ prefixed names from any serialization
      else this->name=name;
      this->mode=mode;
      this->ui_node=ui_node;
      this->type=type;
      this->access=access;
      this->parent = parent;
    }

    inline const ui_base* widget()const{return ui_node;}
    inline const frame* resolve_name(const char* name) const {return _resolve_name(name, this);}
    inline symbol_ret_t resolve_symbol(const char* name) const {return _resolve_symbol(name, this);}
    inline std::pair<const smap<std::string>*,const frame*> resolve_mixin(const char* name) const{return _resolve_mixin(name, this);}
    smap<std::string> compile_mixins(const char* mixins_list) const;


    void signal_to_top(const message_t*){
        //TODO Add logic
    }
    void signal_top_bottom(const message_t*){
        //TODO Add logic
    }

    //Change name for this node.
    void rename(const std::string& new_name){
      if(new_name==name)return;
      else if(parent!=nullptr){
        auto i = parent->children.find(name);
        if(i!=parent->children.end()){
          parent->children.erase(i);
        }
        parent->children.insert(std::pair(new_name,this));
      }
      name = new_name;
    }

    void prune(){
      if(parent!=nullptr){
        parent->children.erase(parent->children.find(name));
        parent=nullptr;
      }
    }

    //Only for local symbols as module ones are not owned
    inline void register_symbol(const char* name, symbol_t value){symbols->insert_or_assign(name,value);}
    inline symbol_t get_symbol(const char* name){const auto& it = symbols->find(name);if(it==symbols->end())return symbol_t::VOID; else return it->second;}
    inline void unregister_symbol(const char* name){symbols->erase(symbols->find(name));}
    inline void reset_symbols(){symbols->clear();}

    /*
    //For top->down signals
    inline void register_filter(const char* name, filter_t value){filters.insert_or_assign(name,value);}
    inline const filter_t get_filter(const char* name){const auto& it = filters.find(name);if(it==filters.end())return nullptr; else return it->second;}
    inline void unregister_filter(const char* name){filters.erase(filters.find(name));}
    inline void reset_filters(){filters.clear();}

    //For bottom->up signals
    inline void register_resolver(const char* name, filter_t value){resolvers.insert_or_assign(name,value);}
    inline const resolver_t get_resolver(const char* name){const auto& it = resolvers.find(name);if(it==resolvers.end())return nullptr; else return it->second;}
    inline void unregister_resolver(const char* name){resolvers.erase(resolvers.find(name));}
    inline void reset_resolver(){resolvers.clear();}
    */

    //TODO helpers for mixin?

    virtual ~frame() {
      //Not needed right now, as frames are destroyed alongside their owners from a list
      return;
      prune();
      //Not very optimized, but it is only going to happen at the end so no worries.
      for(;children.size()!=0;){
        auto i = children.begin();
        delete i->second;
      }
    }
};

}