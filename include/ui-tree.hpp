#pragma once

#include "ui-frame.hpp"
#include <ui.hpp>
#include <cache/commons.hpp>

namespace vs {

struct ui_tree {
  //TODO: Probably not here. I should move it inside the app container class AND a new one unless I want to reuse namespaces
  cache::ctx_t cache_ctx;

  //TODO: Add the path for any dynamic library to be linked with as well.

  //ui_tree* parent = nullptr;      //Filled in for viewports when apps are embedded inside other apps
  enum class type_t{
    NONE, APP, COMPONENT, FRAGMENT
  }type;


  // Globals
  std::string basename;
  std::map<std::string, std::string, std::less<>> modules;
  // TODO Add policies
  std::map<std::string, std::string, std::less<>> props_from_above;
  std::map<std::string, ui<> *, std::less<>> slots_from_above;

  std::vector<ui_base*> nodes;

  // Support during navigation
  frame_mode_t mode = frame_mode_t::AUTO;

  //Run test propagating from the root.

  bool string2key256(const char* str, uint8_t array[256/32] );

  //TODO: It is likely  I want this stuff moved from here.
  //      Also a lot of it might end up in the codegen machine.

  constexpr const char * fl_align_pos_s(Fl_Align v);
  static Fl_Align fl_align_pos_i(const char* v);

  constexpr const char * fl_align_image_s(Fl_Align v);
  static Fl_Align fl_align_image_i(const char* v);

  constexpr const char * fl_align_wrap_s(Fl_Align v);
  static Fl_Align fl_align_wrap_i(const char* v);

  constexpr const char * fl_align_clip_s(Fl_Align v);
  static Fl_Align fl_align_clip_i(const char* v);

  constexpr const char * fl_align_inside_s(Fl_Align v);
  static Fl_Align fl_align_inside_i(const char* v);

  constexpr static const char *fl_font_s(Fl_Font v);
  static Fl_Align fl_font_i(const char *v);

  constexpr static const char *fl_boxtype_s(Fl_Font v);
  static Fl_Boxtype fl_boxtype_i(const char *v);

  constexpr static const char *fl_flex_layout_s(int v);
  static int fl_flex_layout_i(const char *v);

  // Helpers to compute expressions into values that Fl_Widgets can use.

  static bool h_px(uint T, size_t *dst, const char *expr,
                   const ui_base *env);
  static bool h_colour(uint32_t *dst, const char *expr,
                       const ui_base *env);

  static bool h_flag(bool *dst, const char *expr,
                       const ui_base *env);

  virtual ~ui_tree();
  virtual void cleanup();
  virtual int runtime_testsuite();

};


} // namespace vs