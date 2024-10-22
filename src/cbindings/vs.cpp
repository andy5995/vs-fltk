#include <cbindings/vs.h>
#include <iostream>
#include <pugixml.hpp>

void vs_hello_world(){
    std::cout<<"Hello world!\n";
    pugi::xml_document doc;
    doc.load_string("<app><h1></h1></app>");
    std::cout<<doc.root().first_child().name()<<"\n";
}
