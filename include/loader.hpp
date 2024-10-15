#pragma once

class ui_xml_tree;
namespace vs{
struct app_loader{
    ui_xml_tree* root;

    app_loader(const char* path);
    ~app_loader();
    int run();
};
}