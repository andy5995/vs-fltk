#pragma once

#include <cstdint>
namespace vs{


class frame;
class ui_base;

enum class symbol_type_t{
  VOID, UNKNOWN, CALLBACK, DRAW, SETTER, GETTER, DISPATCHER
};

enum class symbol_mode_t{
  VOID,
  NATIVE,
  QUICKJS,
  WASM,
  EXTERNAL,
  LUA,
};

struct symbol_t{
  symbol_mode_t mode:3;
  symbol_type_t type:5;
  const void* symbol;

  static symbol_t VOID;
};

//TODO: Very bad packing. Fix it at some point
struct symbol_ret_t{
  symbol_t symbol;
  symbol_t ctx_apply;
  const frame* found_at;

  typedef ui_base*(*ctx_apply_fn)(ui_base*);
  typedef void(*cb_fn)(ui_base*);
  typedef void(*draw_fn)(); //TODO: to be defined
  typedef int(*set_fn)(const uint8_t* src);
  typedef int(*get_fn)(uint8_t** src);
};

enum class frame_type_t{
  SLOT_CONTAINER,   //Stop any request from moving in between.
  CONTAINER,        //Any bottom up request will stop here.
  SLOT,             //Any top down request will stop here.
  LEAF,             //No children stored in here (but symbols are fine). Go to parent.
  NODE,             //A namespace without constraints on moving in and out
};
enum class frame_access_t{
  PUBLIC,           //Symbols can be searched bottom up & top down.
  PRIVATE,          //There can be symbols, but they are only visible by self. Requests are passed
};

typedef  symbol_mode_t frame_mode_t;

}