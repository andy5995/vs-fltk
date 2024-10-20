#pragma once

#include <memory>
#include <ui.hpp>
#include <tcc-wrap.hpp>
#include <pugixml.hpp>

namespace vs{
namespace bindings{

extern void tcc_error_func_xml(const pugi::xml_node& env, const char * msg);
extern void tcc_log_symbol_func_xml(const pugi::xml_node& env, const char * msg, const char* name);

//extern void tcc_error_func_native(void* env, const char * msg);
//extern void tcc_log_symbol_func_native(void* env, const char * msg, const char* name);


/**
 * @brief Unified function to generate vanilla c code.
 * 
 * @param is_runtime to specify if the component will be run or it is being used in the visual editor
 * @param obj if nullptr it is assumed to be a module.
 * @param src source to compile
 * @param ctx context where this information is anchored (for example the xml node)
 * @param error_fn a function to show tcc errors
 * @param register_fn a function to show symbol registrations
 * @param link_with path of a library to be linked against (exposing the standard vs interface)
 * @return std::shared_ptr<tcc> 
 */
extern std::shared_ptr<tcc> tcc_c_pipeline(bool is_runtime, vs::ui_base* obj, const char* src, void* ctx, void(*error_fn)(void*,const char*), void(*register_fn)(void*,const char*, const char*), const char *link_with);

extern inline std::shared_ptr<tcc> tcc_c_pipeline_xml(bool is_runtime, vs::ui_base* obj, pugi::xml_node& ctx,  const char *link_with){
    return tcc_c_pipeline(is_runtime,obj,ctx.text().as_string(),&ctx,(void(*)(void*,const char*))tcc_error_func_xml,(void(*)(void*,const char*, const char*))tcc_log_symbol_func_xml,link_with);
}



/**
 * @brief 
 * 
 * @param obj the UI widget this is linked to
 * @param component_root the component root. Used to link its symbols.
 * @param node the xml node of the script parent
 * @param is_runtime Handle the case of the script being inside the editor or in the actual runtime.
 * @return std::shared_ptr<tcc> 
 */
extern std::shared_ptr<tcc> tcc_c_pipeline_single_xml(vs::ui_base* obj, vs::ui_base* component_root, const pugi::xml_node& node, const char *link_with, bool is_runtime);

extern std::pair<std::shared_ptr<tcc>,std::shared_ptr<module_symbols>> tcc_c_pipeline_module_xml(const pugi::xml_node& node, bool is_runtime);
}
}