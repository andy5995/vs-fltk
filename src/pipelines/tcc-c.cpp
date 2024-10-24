#include "ui-frame.hpp"
#include "ui.hpp"
#include <cstddef>

#include <pipelines/tcc-c.hpp>
#include <ui-tree.xml.hpp>


namespace vs{
namespace pipelines{ 

void tcc_error_func_xml(const pugi::xml_node& ctx, const char * msg) {
    printf("\n\033[41;37;1m[TCC]\033[0m      : %s @ [\033[93;3m%s\033[0m]", msg,ctx.path().c_str());
}

void tcc_log_symbol_func_xml(const pugi::xml_node& ctx, const char * msg, const char * name) {
    ui_xml_tree::log(severety_t::INFO, ctx , msg, name);
}

/*TODO: 
To be changes so that it can do everything in a single function.
- vs::ui_base* obj if set to nullptr is equivalent to be in a module configuration
- vs::ui_base* component_root is not needed. In the past I wanted to compile along with its script.global, but that is a bad idea.
- const pugi::xml_node& node to be reworked in a generic context. A reference to a valid error function should also be provided as parameters.
- const char *link_with is good
-  bool is_runtime
*/


#define LIB(x)     script->add_sym(#x, (void *)x)

std::shared_ptr<tcc> tcc_c_pipeline(bool is_runtime, vs::ui_base* obj, const char* src, void* ctx, void(*error_fn)(void*,const char*), void(*register_fn)(void*,const char*, const char*), const char *link_with){
    //For now since we don't handel it
    if(obj==nullptr)return nullptr;

    auto script = std::make_shared<tcc>();

    script->set_error_fn(ctx,error_fn);
    script->set_opts("-nostdlib"); //-fno-builtin
    script->set_out_type(tcc::memory);
    script->add_sysinclude_path("./subprojects/libtcc/include/");
    script->add_include_path("./bindings/native/include");

    if(link_with!=nullptr){
        //TODO: Add header path
        //Boilerplate for dlopen?
    }

    // Custom symbol
    script->add_sym("vs_self", (void *)obj);
    script->add_sym("vs_log", (void *)vs_log);
    script->add_sym("vs_resolve_name", (void *)+[](ui_base* w,const char* s){if(w==nullptr)return (const ui_base*)nullptr;return  w->resolve_name(s); });
    script->add_sym("vs_resolve_name_path", (void *)+[](ui_base* w,const char* s){if(w==nullptr)return (const ui_base*)nullptr;return  w->resolve_name_path(s); });
    script->add_sym("vs_resolve_symbol", (void *)+[](ui_base* w,const char* s){if(w==nullptr)return symbol_ret_t {symbol_t::VOID, symbol_t::VOID, nullptr};return w->resolve_symbol(s); });
    script->add_sym("vs_apply_prop", (void *)+[](ui_base* w,const char* k, const char* v){if(w==nullptr)return -1;return w->apply_prop(k,v); });
    script->add_sym("vs_get_computed", (void *)+[](ui_base* w,const char* k, const char** v){if(w==nullptr)return -1;return w->get_computed(k,v); });
    script->add_sym("vs_set", (void *)+[](ui_base* w,const char* k, const char* v){if(w==nullptr)return -1;/*TODO*/ });
    script->add_sym("vs_get", (void *)+[](ui_base* w,const char* k, const char** v){if(w==nullptr)return -1;/*TODO*/ });
    

    // Fragments of stdlib
    // --- cstring
    LIB(memcpy);
    LIB(memcpy);
    LIB(memmove);
    LIB(memset);
    LIB(strcpy);
    LIB(strncpy);
    LIB(strcat);
    LIB(strncat);
    LIB(memcmp);
    LIB(strcmp);

    LIB(strcoll);
    LIB(strncmp);
    LIB(strxfrm);

    LIB((void*(*)(void*, int, size_t))memchr);
    LIB((char*(*)(char*, int))strchr);
    LIB(strcspn);
    LIB((char*(*)(char*, const char* ))strpbrk);
    LIB((char*(*)(char*, int))strrchr);
    LIB(strspn);
    LIB((char*(*)(char*, const char*))strstr);
    LIB(strtok);

    LIB(memset);
    LIB(strerror);
    LIB(strlen);

    // --- cstdlib
    //https://cplusplus.com/reference/cstdlib/
    LIB(calloc);
    LIB(free);
    LIB(malloc);
    LIB(realloc);

    script->add_file("./bindings/native/src/commons.c");
    script->add_file("./bindings/native/src/stub.c");

    script->compile_str_embedded(
        "#include <vs.h>\n#include <stub.h>\n", //TODO: Add custom header if linked with an external thing
        src,
        ""
    );

    script->relocate();

    struct callback_ctx_t{
        ui_base* node_ui;
        void* ref;
        void(*log)(void*,const char*, const char*);
    }cb_ctx{.node_ui=obj, .ref=ctx, .log=register_fn};

    script->ls_sym(&cb_ctx, +[](void* _ctx, const char* name, const void* value){
        callback_ctx_t* ctx = (callback_ctx_t*)_ctx;

        if(strcmp("callback", name)==0){
        ctx->log(ctx->ref,"Registering default callback symbol `%s`",name);
        ctx->node_ui->register_symbol(name, symbol_t{symbol_mode_t::NATIVE,symbol_type_t::CALLBACK,value});
        ctx->node_ui->apply_prop("on.callback", "callback");
        }else if(strcmp("draw", name)==0){
        ctx->log(ctx->ref,"Registering default drawing symbol `%s`",name);
        ctx->node_ui->register_symbol(name, symbol_t{symbol_mode_t::NATIVE,symbol_type_t::DRAW,value});
        ctx->node_ui->apply_prop("on.draw", "draw");
        }else if(strcmp("dispatcher", name)==0){
        ctx->log(ctx->ref,"Registering default dispatching symbol `%s`",name);
        ctx->node_ui->set_dispatcher(symbol_t{symbol_mode_t::NATIVE,symbol_type_t::DRAW,value});
        }else if(strncmp("__EXPORT_CB__", name, 13)==0){
        ctx->log(ctx->ref,"Registering public callback symbol `%s`",name+13);
        ctx->node_ui->register_symbol(name+13, symbol_t{symbol_mode_t::NATIVE,symbol_type_t::CALLBACK,(const void*)(((size_t*)value)[0])});
        }else if(strncmp("__EXPORT_GET_", name, 13)==0){
        ctx->log(ctx->ref,"Registering public getter symbol `%s`",name+13);
        ctx->node_ui->register_symbol(name+13, symbol_t{symbol_mode_t::NATIVE,symbol_type_t::GETTER,(const void*)(((size_t*)value)[0])});
        }else if(strncmp("__EXPORT_SET_", name, 13)==0){
        ctx->log(ctx->ref,"Registering public setter symbol `%s`",name+13);
        ctx->node_ui->register_symbol(name+13, symbol_t{symbol_mode_t::NATIVE,symbol_type_t::GETTER,(const void*)(((size_t*)value)[0])});
        }else if(strncmp("__EXPORT_UKN_", name, 13)==0){
        ctx->log(ctx->ref,"Registering public unknown symbol `%s`",name+13);
        ctx->node_ui->register_symbol(name+13, symbol_t{symbol_mode_t::NATIVE,symbol_type_t::UNKNOWN,(const void*)(((size_t*)value)[0])});
        }
    });
    
    auto on_compiled = (uint64_t(*)())script->get_sym("on_compiled");
    if(on_compiled!=nullptr)on_compiled();
    return script;
}

#undef LIB

}
}