
#pragma once
#include "utils/paths.hpp"
#include "utils/policies.hpp"
#include <ui-tree.hpp>
#include <pugixml.hpp>
#include <globals.hpp>

namespace vs{

struct ui_xml_tree : ui_tree {
private:
    struct ns_t{
        std::string vs = "";             //Namespace for vs basic components (default none, suggested vs: if needed)
        std::string fltk = "fl:";      //Namespace for fltk specific components (default fl:)
        std::string s = "s:";            //Namespace for static xml operations (default s:)
    }ns;

    enum namespaces_t{
        vs, fltk, s
    };

#   if __has_include("ui-tree.xml.ns.autofrag.hpp")
        #include "ui-tree.xml.ns.autofrag.hpp"
        ns_strings strings;
#   endif

    const pugi::xml_node *caller_node = nullptr;
    ui_base* caller_ui_node=nullptr;
    ui_base* root = nullptr;

    pugi::xml_document doc;         //Handle of the xml parser
    policies_t policies;            //Computed policies for this tree
    scoped_rpath_t local;            //Full path for the location of this component.
    scoped_rpath_t fullname;            //Full path for the location of this component.

    std::map<std::string,std::string> imports;
    //std::vector<uint8_t> filebuffer;
    bool is_app;

    size_t local_unique_counter = 0;

public:
    void set_namespace(namespaces_t n, const char* prefix){
        if(n==namespaces_t::vs){if(prefix[0]==0)ns.vs="";else ns.vs=std::string(prefix)+":";}
        else if(n==namespaces_t::fltk){if(prefix[0]==0)ns.fltk="";else ns.fltk=std::string(prefix)+":";}
        else if(n==namespaces_t::s){if(prefix[0]==0)ns.s="";else ns.s=std::string(prefix)+":";}
    }

    template <std::derived_from<ui_base> T>
    T *build_base_widget(const pugi::xml_node &root, ui_base * root_ui = nullptr);

    void _build_base_widget_extended_attr(const pugi::xml_node &root, ui_base * current);
    void _build(const pugi::xml_node &root,  ui_base *root_ui = nullptr);

    int build();

    int load(const char* file, bool app, const pugi::xml_node* caller_node=nullptr, ui_base* caller_ui_node=nullptr, const scoped_rpath_t* caller_path=nullptr, const policies_t& base_policies=globals::policy);

    ~ui_xml_tree();

    // Logging

    static inline FILE *log_device = stdout;
    static void log(int severety, const void *_ctx, const char *str, ...);

    template <class... Args>
    static inline void log(int  severety, const pugi::xml_node &ctx,
                            const char *str, Args... args) {
        log(severety, &ctx, str, args...);
    }
};

}