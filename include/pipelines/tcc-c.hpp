#pragma once

#include <memory>
#include <ui.hpp>
#include <tcc-wrap.hpp>
#include <pugixml.hpp>

namespace vs{
namespace bindings{

//extern void tcc_error_func_native(void* env, const char * msg);
extern void tcc_error_func_xml(const pugi::xml_node& env, const char * msg);

//extern std::shared_ptr<tcc> tcc_c_pipeline_native(vs::ui<>& obj, void* node, bool is_module);

/**
 * @brief 
 * 
 * @param obj the UI widget this is linked to
 * @param component_root the component root. Used to link its symbols.
 * @param node the xml node of the script parent
 * @param is_runtime Handle the case of the script being inside the editor or in the actual runtime.
 * @return std::shared_ptr<tcc> 
 */
extern std::shared_ptr<tcc> tcc_c_pipeline_single_xml(vs::ui_base* obj, vs::ui_base* component_root, const pugi::xml_node& node, bool is_runtime);

extern std::pair<std::shared_ptr<tcc>,std::shared_ptr<module_symbols>> tcc_c_pipeline_module_xml(const pugi::xml_node& node, bool is_runtime);
}
}