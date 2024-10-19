#pragma once

/*
    Glue logic exposed to the final application to avoid exposing too many libraries.
*/

class ui_xml_tree;
namespace vs{
struct app_loader{
    ui_xml_tree* root;

    app_loader(const char* profile, const char* path);
    ~app_loader();
    int run();
};
}