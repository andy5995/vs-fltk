#include "ui-frame.hpp"
#include <cstddef>

#include <pipelines/tcc-c.hpp>
#include <ui-tree.xml.hpp>


namespace vs{
namespace bindings{ 

void tcc_error_func_xml(const pugi::xml_node& env, const char * msg) {
    printf("\n\033[41;37;1m[TCC]\033[0m      : %s @ [\033[93;3m%s\033[0m]", msg,env.path().c_str());
}

std::shared_ptr<tcc> tcc_c_pipeline_single_xml(vs::ui_base* obj, vs::ui_base* component_root, const pugi::xml_node& node, bool is_runtime){
    auto compiler = std::make_shared<tcc>();

    compiler->set_error_fn((void*)&node,(void(*)(void*,const char*))tcc_error_func_xml);
    compiler->set_opts("-nostdlib"); //-fno-builtin
    compiler->set_out_type(tcc::memory);
    compiler->add_sysinclude_path("./subprojects/libtcc/include/");
    compiler->add_include_path("./bindings/native/include");

    ///////////////////////
    //////Add symbols//////
    ///////////////////////

    compiler->add_sym("vs_self", (void *)obj);
    compiler->add_sym("__vs_log", (void *)vs_log);
    //compiler->add_sym("__vs_get_node", (void *)+[](ui<Fl_Widget>* w,const char* s){return w->(s); }
    compiler->add_sym("__vs_resolve_name", (void *)+[](ui_base* w,const char* s){if(w==nullptr)return (const ui_base*)nullptr;return  w->resolve_name(s); });
    compiler->add_sym("__vs_resolve_name_path", (void *)+[](ui_base* w,const char* s){if(w==nullptr)return (const ui_base*)nullptr;return  w->resolve_name_path(s); });
    compiler->add_sym("__vs_resolve_symbol", (void *)+[](ui_base* w,const char* s){if(w==nullptr)return symbol_ret_t {symbol_t::VOID, symbol_t::VOID, nullptr};return w->resolve_symbol(s); });
    compiler->add_sym("__vs_call", (void *)+[](ui_base* w,const char* s, void* ctx){
        if(w==nullptr)return -1;
        //TODO: Unify with the code in the callback assignation.
        auto fn = w->resolve_symbol(s);
        if(fn.ctx_apply.symbol==nullptr)return ((int(*)(void* ctx))fn.symbol.symbol)(ctx);
        else{
            const ui_base* tmp = ((const ui_base*(*)(const ui_base* ctx))fn.ctx_apply.symbol)(fn.found_at->widget());
            int t = ((int(*)(void* ctx))fn.symbol.symbol)(ctx);
            ((const ui_base*(*)(const ui_base* ctx))fn.ctx_apply.symbol)(tmp);
            return t;
        }
    });
    compiler->add_sym("vs_apply_prop", (void *)+[](ui_base* w,const char* k, const char* v){if(w==nullptr)return -1;return w->apply_prop(k,v); });
    compiler->add_sym("vs_get_computed", (void *)+[](ui_base* w,const char* k, const char** v){if(w==nullptr)return -1;return w->get_computed(k,v); });
    //TODO: Add path resolution, a wrapper for fopen to accept only specific paths, and write/read to operate with full files and not streams.


    //cstring
    //compiler->add_sym("itoa", (void *)itoa);
    compiler->add_sym("memcpy", (void *)memcpy);
    compiler->add_sym("memmove", (void *)memmove);
    compiler->add_sym("memset", (void *)memset);
    compiler->add_sym("strcpy", (void *)strcpy);
    compiler->add_sym("strncpy", (void *)strncpy);
    compiler->add_sym("strcat", (void *)strcat);
    compiler->add_sym("strncat", (void *)strncat);
    compiler->add_sym("memcmp", (void *)memcmp);
    compiler->add_sym("strcmp", (void *)strcmp);

    /*
    int strcoll ( const char * str1, const char * str2 );
    int strncmp ( const char * str1, const char * str2, size_t num );
    size_t strxfrm ( char * destination, const char * source, size_t num );

    void * memchr ( const void *, int, size_t ); 
    char * strchr ( const char *, int ); 
    size_t strcspn ( const char * str1, const char * str2 );
    char * strpbrk ( const char *, const char * );
    char * strrchr ( const char *, int ); 
    size_t strspn ( const char * str1, const char * str2 );
    char * strstr ( const char *, const char * ); 
    char * strtok ( char * str, const char * delimiters );

    void * memset ( void * ptr, int value, size_t num );
    char * strerror ( int errnum );
    size_t strlen ( const char * str );
    */

    //cstdlib
    compiler->add_sym("calloc", (void *)calloc);
    compiler->add_sym("free", (void *)free);
    compiler->add_sym("malloc", (void *)malloc);
    compiler->add_sym("realloc", (void *)realloc);

    compiler->add_sym("__vs_log", (void *)vs_log);

    compiler->add_file("./bindings/native/src/commons.c");
    compiler->add_file("./bindings/native/src/stub.c");

    for (auto &i : node.children()) {
        compiler->compile_str_embedded(
            "#include <vs.h>\n#include <stub.h>\n",
            i.text().as_string(""),
            ""
        );
    }

    compiler->relocate();
          

    {
    struct callback_ctx_t{
        ui_base* node_ui;
        const pugi::xml_node& node_xml;
    }ctx{.node_ui =obj, .node_xml =node};

    compiler->ls_sym(&ctx, +[](void* _ctx, const char* name, const void* value){
        callback_ctx_t* ctx = (callback_ctx_t*)_ctx;
        /*if(strncmp("on_", name, 3)==0){
        ui_xml_tree::log(severety_t::INFO, ctx->node_xml, "Registering symbol `%s`",name);
        ctx->node_ui->register_symbol(name, value);
        }*/
        if(strcmp("callback", name)==0){
        ui_xml_tree::log(severety_t::INFO, ctx->node_xml, "Registering default callback symbol `%s`",name);
        ctx->node_ui->register_symbol(name, symbol_t{symbol_mode_t::NATIVE,symbol_type_t::CALLBACK,value});
        ctx->node_ui->apply_prop("on.callback", "callback");
        }else if(strcmp("draw", name)==0){
        ui_xml_tree::log(severety_t::INFO, ctx->node_xml, "Registering default drawing symbol `%s`",name);
        ctx->node_ui->register_symbol(name, symbol_t{symbol_mode_t::NATIVE,symbol_type_t::DRAW,value});
        ctx->node_ui->apply_prop("on.draw", "draw");
        }else if(strcmp("dispatcher", name)==0){
        ui_xml_tree::log(severety_t::INFO, ctx->node_xml, "Registering default dispatching symbol `%s`",name);
        ctx->node_ui->set_dispatcher(symbol_t{symbol_mode_t::NATIVE,symbol_type_t::DRAW,value});
        }else if(strncmp("__EXPORT_CB__", name, 13)==0){
        ui_xml_tree::log(severety_t::INFO, ctx->node_xml, "Registering public callback symbol `%s`",name+13);
        ctx->node_ui->register_symbol(name+13, symbol_t{symbol_mode_t::NATIVE,symbol_type_t::CALLBACK,(const void*)(((size_t*)value)[0])});
        }else if(strncmp("__EXPORT_GET_", name, 13)==0){
        ui_xml_tree::log(severety_t::INFO, ctx->node_xml, "Registering public getter symbol `%s`",name+13);
        ctx->node_ui->register_symbol(name+13, symbol_t{symbol_mode_t::NATIVE,symbol_type_t::GETTER,(const void*)(((size_t*)value)[0])});
        }else if(strncmp("__EXPORT_SET_", name, 13)==0){
        ui_xml_tree::log(severety_t::INFO, ctx->node_xml, "Registering public setter symbol `%s`",name+13);
        ctx->node_ui->register_symbol(name+13, symbol_t{symbol_mode_t::NATIVE,symbol_type_t::GETTER,(const void*)(((size_t*)value)[0])});
        }else if(strncmp("__EXPORT_UKN_", name, 13)==0){
        ui_xml_tree::log(severety_t::INFO, ctx->node_xml, "Registering public unknown symbol `%s`",name+13);
        ctx->node_ui->register_symbol(name+13, symbol_t{symbol_mode_t::NATIVE,symbol_type_t::UNKNOWN,(const void*)(((size_t*)value)[0])});
        }
    });
    }
    auto on_compiled = (uint64_t(*)())compiler->get_sym("on_compiled");
    if(on_compiled!=nullptr)on_compiled();

    return compiler;
}

extern std::pair<std::shared_ptr<tcc>,std::shared_ptr<module_symbols>> tcc_c_pipeline_module_xml(const pugi::xml_node& node){
    //TODO: Add impl
    throw "not implemented";
}

}
}