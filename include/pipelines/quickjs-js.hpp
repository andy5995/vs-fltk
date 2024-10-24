#pragma once

#include <memory>
#include <ui.hpp>
#include <quickjs.h>

#include <pugixml.hpp>

//TODO: restructure following changes in the tcc interface

namespace vs{
namespace pipelines{

/*
struct jsctx{
    private:
        JSContext* data;
    public:
        jsctx(){
            data=JS_NewContext();
        }
};
*/

struct quickjs_t{
    JSContext* ctx;
    std::vector<JSValue> handles;

    operator JSContext* (){return ctx;}

    quickjs_t(JSRuntime* rt){
        ctx=JS_NewContextRaw(rt);
        JS_AddIntrinsicBaseObjects(ctx);
        JS_AddIntrinsicDate(ctx);
        JS_AddIntrinsicEval(ctx);
        JS_AddIntrinsicRegExp(ctx);
        JS_AddIntrinsicJSON(ctx);
        JS_AddIntrinsicProxy(ctx);
        JS_AddIntrinsicMapSet(ctx);
        JS_AddIntrinsicTypedArrays(ctx);
        JS_AddIntrinsicPromise(ctx);
        JS_AddIntrinsicBigInt(ctx);
        JS_AddIntrinsicWeakRef(ctx);

        JS_AddPerformance(ctx);
    
    }

    virtual ~quickjs_t(){
        for(auto& i: handles){JS_FreeValue(ctx, i);}
        JS_FreeContext(ctx);
    }
};

extern std::shared_ptr<quickjs_t> qjs_js_pipeline(bool is_runtime, vs::ui_base* obj, const char* src, void* ctx, void(*error_fn)(void*,const char*), const char *link_with);

extern void qjs_js_pipeline_apply(const std::shared_ptr<quickjs_t>& script,vs::ui_base* obj,void* ctx,void(*register_fn)(void*,const char*, const char*));


/**
 * @brief 
 * 
 * @param obj the UI widget this is linked to
 * @param component_root the component root. Used to link its symbols.
 * @param node the xml node of the script parent
 * @param is_runtime Handle the case of the script being inside the editor or in the actual runtime.
 * @return std::shared_ptr<quickjs_t> 
 */
extern std::shared_ptr<quickjs_t> qjs_js_pipeline_single_xml(vs::ui_base* obj, vs::ui_base* component_root, const pugi::xml_node& node, bool is_runtime);

extern std::pair<std::shared_ptr<quickjs_t>,std::shared_ptr<module_symbols>> qjs_js_pipeline_module_xml(const pugi::xml_node& node, bool is_runtime);
}
}