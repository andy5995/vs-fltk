#pragma once
#include <ui-frame.hpp>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <utils/strings.hpp>

namespace vs{
namespace cache{

struct script_t{
  std::shared_ptr<void> script;
  std::shared_ptr<smap<symbol_t>> symbols;
  frame_mode_t mode;
};

struct buffer_t{
    const uint8_t* data;
    size_t size;
};

enum class resource_t{
            NONE,
            BUFFER,                     //uint8_t[]
            XML_TREE,                   //pugi::xml_document
            SCRIPT,                     //struct script_t
            DYNAMIC_LIB,                //?
            OTHER_RESOURCE,             //?
};

}
}