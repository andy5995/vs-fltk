#pragma once

#include <cstddef>
#include <pugixml.hpp>

namespace vs{


struct policies_t{
    struct networking_t{
        int allow_http: 1;
        int allow_https: 1;
        int allow_gemini: 1;

        void all(){allow_gemini=true;allow_https=true;allow_http=true;}
        void none(){allow_gemini=false;allow_https=false;allow_http=false;}
        void trusted(){allow_gemini=true;allow_https=true;allow_http=true;}
        void normal(){allow_gemini=true;allow_https=true;allow_http=false;}
        void safe(){allow_gemini=false;allow_https=false;allow_http=false;}

        void inherit(const networking_t& parent){allow_gemini&=parent.allow_gemini;allow_https&=parent.allow_https;allow_http&=allow_http;}
    }networking;

    struct embedded_scripts_t{
        int allow_native:1;
        int allow_quickjs:1;
        int allow_wasm:1;

        void all(){allow_wasm=true;allow_quickjs=true;allow_native=true;}
        void none(){allow_wasm=false;allow_quickjs=false;allow_native=false;}
        void trusted(){allow_wasm=true;allow_quickjs=true;allow_native=true;}
        void normal(){allow_wasm=true;allow_quickjs=true;allow_native=false;}
        void safe(){allow_wasm=true;allow_quickjs=true;allow_native=false;}

        void inherit(const embedded_scripts_t& parent){allow_wasm&=parent.allow_wasm;allow_quickjs&=parent.allow_quickjs;allow_native&=allow_native;}
    }embedded_scripts;

    int allow_native_components: 1;
    int allow_networking: 1;
    int allow_fs: 1;
    int allow_themes: 1;
    int allow_notify: 1;
    int allow_permanent_storage: 1;

    int verbosity:3;
    //allow_caching

    void all(){networking.all();embedded_scripts.all();allow_native_components=true;allow_networking=true;allow_fs=true;allow_themes=true;allow_notify=true;allow_permanent_storage=true;}
    void none(){networking.none();embedded_scripts.none();allow_native_components=false;allow_networking=false;allow_fs=false;allow_themes=false;allow_notify=false;allow_permanent_storage=false;}
    void trusted(){networking.trusted();embedded_scripts.trusted();allow_native_components=true;allow_networking=false;allow_fs=false;allow_themes=true;allow_notify=true;allow_permanent_storage=true;}
    void normal(){networking.normal();embedded_scripts.normal();allow_native_components=true;allow_networking=false;allow_fs=false;allow_themes=true;allow_notify=true;allow_permanent_storage=true;}
    void safe(){networking.safe();embedded_scripts.safe();allow_native_components=false;allow_networking=false;allow_fs=false;allow_themes=false;allow_notify=true;allow_permanent_storage=false;}

    void inherit(const policies_t& parent){
        networking.inherit(parent.networking);
        embedded_scripts.inherit(parent.embedded_scripts);
        allow_native_components&=parent.allow_native_components;
        allow_networking&=parent.allow_networking;
        allow_fs&=allow_fs;
        allow_themes&=allow_themes;
        allow_notify&=allow_notify;
        allow_permanent_storage&=allow_permanent_storage;
        verbosity=1;
    }

    policies_t(){all();}

    void inherit_from_env();
    void inherit_from_xml(const pugi::xml_node& root);
    void save_to_xml(pugi::xml_node root);
};


}