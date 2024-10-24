#include "quickjs.h"
#include "ui-frame.hpp"
#include "ui.hpp"
#include "utils/env.hpp"
#include <memory>
#include <ui-tree.xml.hpp>
#include <pipelines/quickjs-js.hpp>

namespace vs{
namespace pipelines{ 

static void qjs_error_func_xml(const pugi::xml_node& env, const char * msg) {
    printf("\n\033[41;37;1m[QJS]\033[0m      : %s @ [\033[93;3m%s\033[0m]", msg,env.path().c_str());
}

/*
static JSValue js_print_to_console(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    int i;
    const char* str;
    size_t len;

    for (i = 0; i < argc; i++) {
        if (i != 0) fputc(' ', stdout);
        str = JS_ToCStringLen(ctx, &len, argv[i]);
        if (!str) return JS_EXCEPTION;
        fwrite(str, 1, len, stdout);
        JS_FreeCString(ctx, str);
    }
    fputc('\n', stdout);
    return JS_UNDEFINED;
}


static JSValue js_export(JSContext * ctx , JSValueConst this_val, int argc, JSValueConst* argv){
    if(argc!=1){
        throw "Cannot accept export of zero functions";
    }
    if(JS_IsFunction(ctx, argv[0])){
        auto ret = JS_Call(ctx, argv[0], this_val, 0, nullptr);
        JS_FreeValue(ctx,ret);
    }
    return JS_UNDEFINED;
}
*/

/*Cannot handle additional args. Use string interpolation from JS*/
static JSValue js_log(JSContext * ctx , JSValueConst this_val, int argc, JSValueConst* argv){
    if(argc<3){
        return JS_UNDEFINED;     
    }
    if(JS_IsNumber(argv[0]) && JS_IsBigInt(ctx,argv[1]) && JS_IsString(argv[2])){
        uint32_t severety;
        uint64_t node;
        const char* str;
        JS_ToUint32(ctx, &severety, argv[0]);
        JS_ToBigUint64(ctx, &node, argv[1]);
        str = JS_ToCString(ctx, argv[2]);

        vs_log(severety,(const ui_base*)node,str);

        JS_FreeCString(ctx, str);
    }
    return JS_UNDEFINED;
}

static JSValue js_resolve_name_path(JSContext * ctx , JSValueConst this_val, int argc, JSValueConst* argv){
    if(argc<2){
        return JS_UNDEFINED;
    }
    if(JS_IsBigInt(ctx,argv[0]) && JS_IsString(argv[1])){
        uint64_t node;
        const char* str;
        JS_ToBigUint64(ctx, &node, argv[0]);
        str = JS_ToCString(ctx, argv[1]);
        auto t = JS_NewBigUint64(ctx,(uint64_t)((ui_base*)node)->resolve_name_path(str));
        JS_FreeCString(ctx, str);
        return t;

    }

    return JS_UNDEFINED;
}

static JSValue js_apply_prop(JSContext * ctx , JSValueConst this_val, int argc, JSValueConst* argv){
    if(argc<3){
        return JS_UNDEFINED;
    }

    if(JS_IsBigInt(ctx,argv[0]) && JS_IsString(argv[1]) && JS_IsString(argv[2])){
        uint64_t node;
        const char* key;
        const char* value;

        JS_ToBigUint64(ctx, &node, argv[0]);
        key = JS_ToCString(ctx, argv[1]);
        value = JS_ToCString(ctx, argv[2]);

        auto t = JS_NewInt32(ctx,(uint64_t)((ui_base*)node)->apply_prop(key,value));
        JS_FreeCString(ctx, key);
        JS_FreeCString(ctx, value);
        return t;

    }
    return JS_UNDEFINED;
}

static JSValue js_get_computed(JSContext * ctx , JSValueConst this_val, int argc, JSValueConst* argv){
    if(argc<2){
        return JS_UNDEFINED;
    }

    if(JS_IsBigInt(ctx,argv[0]) && JS_IsString(argv[1]) && JS_IsString(argv[2])){
        uint64_t node;
        const char* key;
        const char* value;

        JS_ToBigUint64(ctx, &node, argv[0]);
        key = JS_ToCString(ctx, argv[1]);

        ((ui_base*)node)->get_computed(key,&value);
        auto t = JS_NewString(ctx,value);
        JS_FreeCString(ctx, key);
        JS_FreeCString(ctx, value);
        delete[] value;
        return t;

    }
    return JS_UNDEFINED;
}

static JSValue js_fake(JSContext * ctx , JSValueConst this_val, int argc, JSValueConst* argv){
    return JS_UNDEFINED;
}

/**
 * @brief Append hooks on the ctx
 * 
 * @param ctx 
 * @param self 
 * @param is_runtime if true real hooks are added; false for test mode (like within the editor)
 */
static void init_c_hooks(JSContext* ctx, void* self, bool is_runtime) {
    JSValue global_obj = JS_GetGlobalObject(ctx);

    JSValue core_obj = JS_NewObject(ctx);

    JS_SetPropertyStr(ctx, core_obj, "self", JS_NewBigUint64(ctx,(size_t)self));
    if(is_runtime){
        JS_SetPropertyStr(ctx, core_obj, "log", JS_NewCFunction(ctx, js_log, "log", 3));
        JS_SetPropertyStr(ctx, core_obj, "resolve_name_path", JS_NewCFunction(ctx, js_resolve_name_path, "resolve_name_path", 2));
        JS_SetPropertyStr(ctx, core_obj, "apply_prop", JS_NewCFunction(ctx, js_apply_prop, "apply_prop", 3));
        JS_SetPropertyStr(ctx, core_obj, "get_computed", JS_NewCFunction(ctx, js_get_computed, "get_computed", 2));
        //JS_SetPropertyStr(ctx, core_obj, "call", core__call);
        //JS_SetPropertyStr(ctx, core_obj, "set", core__set);
        //JS_SetPropertyStr(ctx, core_obj, "get", core__get);
    }
    else{
        JS_SetPropertyStr(ctx, core_obj, "log", JS_NewCFunction(ctx, js_fake, "log", 3));
        JS_SetPropertyStr(ctx, core_obj, "resolve_name_path", JS_NewCFunction(ctx, js_fake, "resolve_name_path", 2));
        JS_SetPropertyStr(ctx, core_obj, "apply_prop", JS_NewCFunction(ctx, js_fake, "apply_prop", 3));
        JS_SetPropertyStr(ctx, core_obj, "get_computed", JS_NewCFunction(ctx, js_fake, "get_computed", 2));
        //JS_SetPropertyStr(ctx, core_obj, "call", core__call);
        //JS_SetPropertyStr(ctx, core_obj, "set", core__set);
        //JS_SetPropertyStr(ctx, core_obj, "get", core__get);
    }


    JS_SetPropertyStr(ctx, global_obj, "core", core_obj);


    //JSValue console_obj = JS_NewObject(ctx);
    //JS_SetPropertyStr(ctx, console_obj, "log",JS_NewCFunction(ctx, js_print_to_console, "log", 1));
    //JS_SetPropertyStr(ctx, global_obj, "console", console_obj);

    JS_FreeValue(ctx, global_obj);
}   


static void dump_value_to_stream(JSContext* ctx,const pugi::xml_node& node, JSValueConst val) {
    const char* strval = JS_ToCString(ctx, val);
    if (strval) {
        qjs_error_func_xml(node,strval);
        JS_FreeCString(ctx, strval);
    } else {
        qjs_error_func_xml(node,"[exception]");
    }
}


extern std::shared_ptr<quickjs_t> qjs_js_pipeline_single_xml(vs::ui_base* obj, vs::ui_base* component_root, const pugi::xml_node& node, bool is_runtime){
    //TODO: If using the cache I can duplicate from one already mostly defined.
    //std::shared_ptr<quickjs_t> _ctx = std::make_shared<quickjs_t>((JSRuntime*)(global_js_rt()));
    std::shared_ptr<quickjs_t> _ctx = std::shared_ptr<quickjs_t>( new quickjs_t((JSRuntime*)(global_js_rt())), +[](void* o){delete (quickjs_t*)o;});


    auto& ctx = *_ctx;

    if (!ctx) {
        qjs_error_func_xml(node, "Unable to create a new js context");
        return nullptr;
    }

    //Decide if I want to include stuff from the js stdlib

    init_c_hooks(ctx, obj, is_runtime);

    const char prefix[] = {
#embed "../../bindings/quickjs/vs.js"       
    };

    for (auto &i : node.children()) {
        const char* program =i.text().as_string("");

        auto result = JS_Eval(ctx, prefix, strlen(prefix), "<input>", JS_EVAL_TYPE_GLOBAL);
        result = JS_Eval(ctx, program, strlen(program), "<input>", JS_EVAL_TYPE_GLOBAL);
        if (JS_IsException(result)) {
            JSValue exception = JS_GetException(ctx);
            JS_BOOL is_error = JS_IsError(ctx, exception);
            dump_value_to_stream(ctx, node, exception);
            if (is_error) {
                JSValue stack = JS_GetPropertyStr(ctx, exception, "stack");
                if (!JS_IsUndefined(stack)) {
                    dump_value_to_stream(ctx, node, stack);
                }
                JS_FreeValue(ctx, stack);
            }
            
            JS_FreeValue(ctx, exception);
        }

        JS_FreeValue(ctx,result);
    }

    JSPropertyEnum* list = nullptr;
    uint32_t list_items;
    auto globalThis = JS_GetGlobalObject(ctx);
    //auto module = JS_GetPropertyStr(ctx,globalThis,"this");

    JS_GetOwnPropertyNames(ctx,&list,&list_items, globalThis,JS_GPN_STRING_MASK|JS_GPN_SYMBOL_MASK|JS_GPN_PRIVATE_MASK);

    for(int i=0;i<list_items;i++){
        auto name = JS_AtomToCString(ctx, list[i].atom);

        if(strcmp("callback", name)==0){
            ui_xml_tree::log(severety_t::INFO, node, "Registering default callback symbol `%s`",name);
            auto ref = JS_GetPropertyStr(ctx, globalThis, name);
            ctx.handles.push_back(ref);
            obj->register_symbol(name,symbol_t{symbol_mode_t::QUICKJS,symbol_type_t::CALLBACK,(void*)(ctx.handles.size())});
            obj->apply_prop("on.callback", "callback");
        }
        else if(strcmp("draw", name)==0){
            ui_xml_tree::log(severety_t::INFO, node, "Registering default drawing symbol `%s`",name);
            auto ref = JS_GetPropertyStr(ctx, globalThis, name);
            ctx.handles.push_back(ref);
            obj->register_symbol(name,symbol_t{symbol_mode_t::QUICKJS,symbol_type_t::DRAW,(void*)(ctx.handles.size())});
            obj->apply_prop("on.draw", "draw");
        }
        else if(strcmp("dispatcher", name)==0){
            ui_xml_tree::log(severety_t::INFO, node, "Registering default dispatching symbol `%s`",name);
            auto ref = JS_GetPropertyStr(ctx, globalThis, name);
            ctx.handles.push_back(ref);
            obj->set_dispatcher(symbol_t{symbol_mode_t::QUICKJS,symbol_type_t::DISPATCHER,(void*)(ctx.handles.size())});
        }
        else if(strncmp("__EXPORT_CB__", name, 13)==0){
            ui_xml_tree::log(severety_t::INFO, node, "Registering public callback symbol `%s`",name+13);
            auto ref = JS_GetPropertyStr(ctx, globalThis, name);
            ctx.handles.push_back(ref);
            obj->register_symbol(name+13, symbol_t{symbol_mode_t::QUICKJS,symbol_type_t::CALLBACK,(void*)(ctx.handles.size())});
        }
        else if(strncmp("__EXPORT_GET__", name, 13)==0){
            ui_xml_tree::log(severety_t::INFO, node, "Registering public getter symbol `%s`",name+13);
            auto ref = JS_GetPropertyStr(ctx, globalThis, name);
            ctx.handles.push_back(ref);
            obj->register_symbol(name+13, symbol_t{symbol_mode_t::QUICKJS,symbol_type_t::GETTER,(void*)(ctx.handles.size())});
        }
        else if(strncmp("__EXPORT_SET__", name, 13)==0){
            ui_xml_tree::log(severety_t::INFO, node, "Registering public setter symbol `%s`",name+13);
            auto ref = JS_GetPropertyStr(ctx, globalThis, name);
            ctx.handles.push_back(ref);
            obj->register_symbol(name+13, symbol_t{symbol_mode_t::QUICKJS,symbol_type_t::GETTER,(void*)(ctx.handles.size())});
        }
        else if(strncmp("__EXPORT_UKN__", name, 13)==0){
            ui_xml_tree::log(severety_t::INFO, node, "Registering public unknown symbol `%s`",name+13);
            auto ref = JS_GetPropertyStr(ctx, globalThis, name);
            ctx.handles.push_back(ref);
            obj->register_symbol(name+13, symbol_t{symbol_mode_t::QUICKJS,symbol_type_t::UNKNOWN,(void*)(ctx.handles.size())});
        }
        JS_FreeCString(ctx, name);
    }

    JS_FreeValue(ctx, globalThis);
    JS_FreePropertyEnum(ctx, list, list_items);

    return _ctx;
}

}
}