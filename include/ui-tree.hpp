#pragma once

#include "ui-frame.hpp"
#include <ui.hpp>

namespace vs {
struct ui_script {
  std::string content;
  enum { FILE, INLINE, CACHE } type;
};

struct ui_tree {
  struct{
    uint8_t src_key[256/8];       //Key as it appears on the app definition
    uint8_t computed_key[256/8];  //Key after the parent one has been applied. Root keys is the one of the parent. Needed to avoid nasty secrets violations
    std::string page_tag;         //Used to identify different apps all sharing the same key. While permanent data is shared, secrets are tagged to match a specific "page"
    //TODO: Add dynamic library to be linked as well.
  };

  // Globals
  std::string basename;
  std::map<std::string, std::string, std::less<>> modules;
  // TODO Add policies
  std::map<std::string, std::string, std::less<>> props_from_above;
  std::map<std::string, ui<> *, std::less<>> slots_from_above;

  std::vector<ui_base*> nodes;

  //These no longer exists.
  //ui_script global_decl;
  //ui_script global_def;

  // Support during navigation
  frame_mode_t mode = frame_mode_t::AUTO;


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

  // virtual void load_from_path(const char* path, bool root);
  // virtual void load_from_cache(uuid_t uuid, bool root);

  ~ui_tree();
};


} // namespace vs