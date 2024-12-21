#pragma once

#include <utils/app-env.hpp>
#include <utils/env.hpp>
#include <utils/paths.hpp>
#include <utils/policies.hpp>
#include <cache/memory-storage.hpp>
#include <cache/res-storage.hpp>
#include <cache/kv-storage.hpp>
#include <cache/secrets.hpp>

namespace vs{

namespace singleton{

#ifdef VS_USE_NETWORKING_CURL
struct curl_t{
    curl_t();
    ~curl_t();
} extern curl;
#endif

struct debug_t{
    FILE* fd = nullptr;
    debug_t();
    ~debug_t();

    void operator()(const char* field, const char* value);
} extern debug;

struct field_models_t{
    field_model_t models[];

    enum{
        UNKNOWN, FLAG, ENUM, RAW, PATH, STRING, COLOR, SCALAR_1, SCALAR_2, SCALAR_4
    };
} extern field_models;
}