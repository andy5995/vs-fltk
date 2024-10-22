#pragma once
/*
    vs-fltk is a library meant to be used with its C++ interface.  
    Only a subset of its functionality is exposed with proper C bindings.  
    Basically, what is needed to compile XML components into C, run them, 
    exposing the limited runtime DOM-like structure, resolving paths and little more.
*/

#ifdef __cplusplus
extern "C"{
#endif

typedef void vs_frame;
typedef void vs_ui_base;

#if __has_include("./components.autogen.h")
    #include "./components.autogen.h"
#endif

extern void vs_hello_world();

#ifdef __cplusplus
}
#endif