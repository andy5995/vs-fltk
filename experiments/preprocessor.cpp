#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <optional>
#include <stack>
#include <string>
#include <vector>
#include <pugixml.hpp>

#define PREFIX_STR(name,value) char name[ns_prefix_len+std::char_traits<char>::length(value)+1];memcpy((void*)value,ns_prefix,ns_prefix_len);memcpy((void*)&value[ns_prefix_len],value,std::char_traits<char>::length(value));name[ns_prefix_len+std::char_traits<char>::length(value)]=0;


/*
To test:  ./build/experiments/static-preprocessor ./examples/static-building/data.xml ./examples/static-building/template.xml
*/

const char* ns_prefix = "s:";
size_t ns_prefix_len = strlen(ns_prefix);

//Utility class to implement a list of symbols. Use for `for` like structures in pattern matching.
struct symbol_map{
    private:
        std::vector<std::map<std::string,std::string>> symbols;
    public:
        symbol_map(){
            new_frame();
        }

        void new_frame(){
            symbols.push_back({});
        };

        void remove_frame(){
            symbols.pop_back();
        };

        const char* resolve(const char* name){
            for(auto it = symbols.rbegin();it!=symbols.rend();it++){
                auto found = it->find(name);
                if(found!=it->end()){return found->second.c_str();}
            }
            return nullptr;
        }

        void set(const std::string& name,const std::string& value){
            symbols.back().emplace(name,value);
        }

        struct guard_t{
            private:
                symbol_map& ref;
            public:
                inline guard_t(symbol_map& r):ref(r){ref.new_frame();}
                inline ~guard_t(){ref.remove_frame();}
        };

        inline guard_t guard(){return guard_t(*this);}
};

constexpr std::size_t cx_strlen(const char* s){
    return std::char_traits<char>::length(s);
}

inline bool cx_strneqv(const char* s, const char* c){
    return strncmp(s, c, cx_strlen(c))==0;
}

struct document{
    pugi::xml_document compiled;

    std::stack<pugi::xml_node> stack_compiled;
    std::stack<pugi::xml_node_iterator> stack_data;
    std::stack<std::pair<pugi::xml_node_iterator,pugi::xml_node_iterator>> stack_template;
    symbol_map symbols;

    document(const pugi::xml_document& root_data, const pugi::xml_document& root_template){
        stack_template.push({root_template.root().begin(),root_template.root().end()});
        stack_compiled.push(compiled.root());
    }

    pugi::xml_document& parse (){
        _parse({});
        return compiled;
    }

    std::optional<std::string> eval_expr(const std::string& expr){
        auto t =symbols.resolve(expr.c_str());
        if(t==nullptr)return {};
        return t;
    }

    void _parse(std::optional<pugi::xml_node_iterator> stop_at){ 
        /*
        PREFIX_STR(FOR_RANGE_TAG,"for-range");
        PREFIX_STR(FOR_TAG,"for");
            PREFIX_STR(EMPTY_TAG,"empty");
            PREFIX_STR(HEADER_TAG,"header");
            PREFIX_STR(FOOTER_TAG,"footer");
            PREFIX_STR(BODY_TAG,"body");
        PREFIX_STR(WHEN_TAG,"when");
            PREFIX_STR(CASE_TAG,"case");
        */
        while(!stack_template.empty()){
            auto& current_template = stack_template.top();
            auto& current_compiled = stack_compiled.top();

            if(stop_at.has_value() && current_template.first==stop_at)break;

            if(current_template.first!=current_template.second){
                //Special handling of static element
                if(strncmp(current_template.first->name(), ns_prefix, ns_prefix_len)==0){
                    if(strcmp(current_template.first->name()+ns_prefix_len,"for-range")==0){
                        const char* tag = current_template.first->attribute("tag").as_string("$");
                        int from = current_template.first->attribute("from").as_int(0);
                        int to = current_template.first->attribute("to").as_int(0);
                        int step = current_template.first->attribute("step").as_int(1);
                        for(int i=from; i<to; i+=step){
                            auto frame_guard = symbols.guard();
                            symbols.set(tag,std::to_string(i));
                            stack_template.push({current_template.first->children().begin(),current_template.first->children().end()});
                            _parse(current_template.first);
                            //When exiting one too many is removed. restore it.
                            stack_compiled.push(current_compiled);
                        }
                    }
                    else if(strcmp(current_template.first->name()+ns_prefix_len,"for")==0){
                        const char* tag = current_template.first->attribute("tag").as_string("$");
                        const char* in = current_template.first->attribute("in").as_string();
                        //filter
                        //sort-by
                        //order-by
                        int limit = current_template.first->attribute("limit").as_int(-1);
                        int offset = current_template.first->attribute("offset").as_int(0);

                        bool empty=false;
                        if(empty){
                            //TODO:Test this
                            PREFIX_STR(EMPTY_TAG,"empty");
                            for(const auto& el: current_template.first->children(EMPTY_TAG)){
                                stack_template.push({el.children().begin(),el.children().end()});
                                _parse(el);
                                stack_compiled.push(el);
                            }
                        }
                        else{
                            //Header (once)
                            //Items (iterate)
                            //Footer (once)
                        }
                    }
                    else if(strcmp(current_template.first->name()+ns_prefix_len,"for-prop")==0){}
                    else if(strcmp(current_template.first->name()+ns_prefix_len,"element")==0){}
                    else if(strcmp(current_template.first->name()+ns_prefix_len,"eval")==0){
                        auto tmp = current_compiled.append_child(pugi::node_pcdata);
                        auto val = eval_expr(current_template.first->text().as_string("$"));
                        if(val.has_value()){
                            tmp.set_value(val->c_str());
                        }
                        //current_compiled.append_buffer(current_template.first->text().as_string(),strlen(current_template.first->text().as_string()));
                    }
                    else if(strcmp(current_template.first->name()+ns_prefix_len,"use")==0){}
                    else if(strcmp(current_template.first->name()+ns_prefix_len,"when")==0){}
                    else {std::cerr<<"unrecognized static operation\n";}
                    current_template.first++;
                    continue;
                }

                auto last = current_compiled.append_child(current_template.first->type());
                last.set_name(current_template.first->name());
                last.set_value(current_template.first->value());
                for(const auto& attr :current_template.first->attributes()){
                    //Special handling of static attribute rewrite rules
                    if(strncmp(attr.name(), ns_prefix, ns_prefix_len)==0){
                        if(cx_strneqv(attr.name()+ns_prefix_len,"for.src.")){}
                        else if(cx_strneqv(attr.name()+ns_prefix_len,"for-prop.src.")){}
                        else if(cx_strneqv(attr.name()+ns_prefix_len,"use.src.")){}
                        else if(cx_strneqv(attr.name()+ns_prefix_len,"eval.")){}
                        else {std::cerr<<"unrecognized static operation\n";}
                    }
                    else last.append_attribute(attr.name()).set_value(attr.value());
                }
                if(!current_template.first->children().empty()){
            
                    stack_template.push({current_template.first->children().begin(),current_template.first->children().end()});
                    stack_compiled.push(last);
                }
                current_template.first++;
            }
            else{
                stack_template.pop();
                stack_compiled.pop();
            }
        }

        return;
    }

};


int main(int argc, const char* argv[]){
    if(argc<3){
        std::cerr<<"Wrong usage. "<<argv[0]<<" DATA TEMPLATE [PREFIX=`s:`]\n";
        exit(1);
    }

    pugi::xml_document data, tmpl;
    data.load_file(argv[1]);
    tmpl.load_file(argv[2]);
    if(argc==4){ns_prefix=argv[3];ns_prefix_len=strlen(ns_prefix);}
    
    document doc(data,tmpl);
    doc.parse().save(std::cout);
    return 0;
}

#undef PREFIX_STR