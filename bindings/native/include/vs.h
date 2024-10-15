#pragma once
/**
 * @file vs.h 
 * @author karurochari (public@karurochari.com)
 * @brief Main interface of bindings between c-native scripts and vs
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdarg.h>

typedef  void* self_t;

extern self_t vs_self;
#define $self &vs_self

enum log_severity_t{
  LOG_INFO,
  LOG_OK,
  LOG_WARNING,
  LOG_CONTINUE,
  LOG_PANIC,
  LOG_LOG,
};
typedef enum log_severety_t log_severety_t;


struct symbol_t{
  enum{
    UNKNOWN=1, CALLBACK, DRAW, SETTER, GETTER,
  } mode:3;
  enum{
    NATIVE=1, QUICKJS, WASM
  } type:5;
  const void* symbol;
};

typedef struct symbol_t symbol_t;


struct symbol_ret_t{
  symbol_t symbol;
  symbol_t ctx_apply;
  const void* found_at; //TODO: Change to self.
};

typedef struct symbol_ret_t symbol_ret_t;


extern int printf(const char*restrict, ...);

extern int __vs_log(int severety, self_t self, const char* string, ...);
#define $$log(self,sev,string, ...) __vs_log(sev,self,string, ##__VA_ARGS__)
#define $log(sev,string, ...) __vs_log(sev,&vs_self,string, ##__VA_ARGS__)


extern self_t __vs_resolve_name(self_t, const char* string);
#define vs_resolve_name(string) __vs_resolve_name(&vs_self,string)
extern self_t __vs_resolve_name_path(self_t, const char* string);
#define $$(self,string) __vs_resolve_name_path(self,string)
#define $(string) __vs_resolve_name_path(&vs_self,string)

extern symbol_ret_t __vs_resolve_symbol(self_t, const char* string);
#define vs_resolve_symbol(string) __vs_resolve_symbol(&vs_self,string)

extern int vs_apply_prop(self_t, const char* k, const char* v);
extern int vs_get_computed(self_t, const char* k, const char** v);
#define $$prop(self,key,value) vs_apply_prop(self,key,value)
#define $$computed(self,key,value) vs_get_computed(self,key,value)
#define $$mixin(self,value) vs_apply_prop(self,"mixin",value)
#define $prop(key,value) vs_apply_prop(&vs_self,key,value)
#define $computed(key,value) vs_get_computed(&vs_self,key,value)
#define $mixin(value) vs_apply_prop(&vs_self,"mixin",value)

extern int __vs_call(self_t, const char*, void* args);
#define $$call(self,name, ...) __vs_call(self, name, ##__VA_ARGS__)
#define $call(name, ...) __vs_call(&vs_self, name, ##__VA_ARGS__)

//Utility functions to export symbols
#define $callback(x)  void* __EXPORT_CB__##x = &x;
#define $cb(x)        void* __EXPORT_CB__##x = &x;
#define $plotter(x)   void* __EXPORT_DRW_##x = &x;
#define $getter(x)    void* __EXPORT_GET_##x = &x;
#define $setter(x)    void* __EXPORT_SET_##x = &x;
#define $fn(x)        void* __EXPORT_UKN_##x = &x;



/*
struct vs_context_t{
    void* xml_node;
    void* ui_node;
};
typedef struct vs_context_t vs_context_t;

typedef void Fl_Widget;
typedef void (*vs_callback_t)(Fl_Widget*, void *);

#define PUBLIC(x) x, *__EXPORT__##x = &x
#define export(x) void* __EXPORT__##x = (void*)&x;


extern vs_context_t ctx;
*/