#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <optional>
#include <stack>
#include <string>
#include <variant>
#include <vector>
#include <pugixml.hpp>


/*
To test:  ./build/experiments/static-preprocessor ./examples/static-building/data.xml ./examples/static-building/template.xml
*/


#define PREFIX_STR(name,value) char name[ns_prefix_len+std::char_traits<char>::length(value)+1];memcpy(name,ns_prefix,ns_prefix_len);memcpy(name+ns_prefix_len,value,std::char_traits<char>::length(value));name[ns_prefix_len+std::char_traits<char>::length(value)]=0;

template<typename T>
const T& get_or(const auto& ref, const T& defval) noexcept{
    if(std::holds_alternative<T>(ref))return std::get<T>(ref);
    else return defval;
}

const char* ns_prefix = "s:";
size_t ns_prefix_len = strlen(ns_prefix);



//Utility class to implement a list of symbols. Use for `for` like structures in pattern matching.
struct symbol_map{
    private:
        std::vector<std::map<std::string,std::variant<int,const pugi::xml_node, const pugi::xml_attribute>>> symbols;
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

        std::string resolve_str(const char* name){
            for(auto it = symbols.rbegin();it!=symbols.rend();it++){
                auto found = it->find(name);
                if(found!=it->end()){
                    if(std::holds_alternative<int>(found->second))return std::to_string(std::get<int>(found->second));
                    else if(std::holds_alternative<const pugi::xml_node>(found->second))return std::string(std::get<const pugi::xml_node>(found->second).text().as_string());
                    else if(std::holds_alternative<const pugi::xml_attribute>(found->second))return std::string(std::get<const pugi::xml_attribute>(found->second).as_string());
                }
            }
            return nullptr;
        }

        std::optional<std::variant<int,const pugi::xml_node, const pugi::xml_attribute>> resolve(std::string_view name){
            std::cout<<"Searching "<<name<<"\n";
            for(auto it = symbols.rbegin();it!=symbols.rend();it++){
                auto found = it->find(std::string(name));
                if(found!=it->end()){
                    return found->second;
                }
            }
            return {};
        }

        void set(const std::string& name, auto value){
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

    pugi::xml_node root_data;

    document(const pugi::xml_document& root_data, const pugi::xml_document& root_template){
        stack_template.push({root_template.root().begin(),root_template.root().end()});
        stack_compiled.push(compiled.root());
        this->root_data=root_data.root();
    }

    pugi::xml_document& parse (){
        _parse({});
        return compiled;
    }

    /*
    Allowed formats (for now):
    {node}/...~attr
    {node}/.../
    /.../
    /...~attr
    */
    std::optional<std::variant<int,const pugi::xml_node, const pugi::xml_attribute, std::string>> resolve_expr(const char* _str, int limit = -1){
        int str_len = strlen(_str);
        char str[str_len+1];
        memcpy(str,_str,str_len+1);

        pugi::xml_node ref;
        int idx = 0;
        if(str[0]=='.' || str[0]=='+' || str[0]=='-' || (str[0]>'0' && str[0]<'9')) return atoi(_str);
        else if(str[0]=='{'){
            int close = 0;
            for(;close<str_len && str[close]!='}';close++);
            str[close]=0;
            auto tmp = symbols.resolve(std::string_view(str+1,str+close));
            if(!tmp.has_value())return {};
            else if(std::holds_alternative<const pugi::xml_node>(tmp.value())){
                ref=std::get<const pugi::xml_node>(tmp.value());
            }
            else if(std::holds_alternative<int>(tmp.value())){
                return std::get<int>(tmp.value());
            }
            else if(std::holds_alternative<const pugi::xml_attribute>(tmp.value())){
                return std::get<const pugi::xml_attribute>(tmp.value());
            }
            if(close==str_len)return ref;        //End of string was met earlier
            idx=close+1;
        }
        else if(str[0]=='$'){

            auto tmp =  symbols.resolve("$");
            if(!tmp.has_value() || std::holds_alternative<const pugi::xml_node>(tmp.value())==false)return {};
            else{
                ref=std::get<const pugi::xml_node>(tmp.value());
            }
            if(str[1]==0)return ref;            //End of string was met earlier
            idx++;
        }
        else if(str[0]=='/'){
            ref=root_data;
            idx++;
        }


        for(;;){
            int close = idx;
            for(;close<str_len && str[close]!='/' && str[close]!='~';close++);
            str[close]=0;
            if(idx!=close)ref = ref.child(str+idx); //Avoid the prefix /
            if(close==str_len )return ref;            //End of string was met earlier
            else if(str[close]=='~'){idx=close;break;}
            else{idx=close+1;}
        }
        //Process the terminal attribute
        if(str[idx]=='~'){
            return ref.attribute(str+idx+1).as_string();
        }

        return {};
    }

    //TODO: use resolve_expr
    std::optional<std::string> eval_expr(const std::string& expr){
        auto t =symbols.resolve_str(expr.c_str());
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
                        const char* tag = current_template.first->attribute("tag").as_string();
                        int from = get_or<int>(resolve_expr(current_template.first->attribute("from").as_string("0")).value_or(0),0);
                        int to = get_or<int>(resolve_expr(current_template.first->attribute("to").as_string("0")).value_or(0),0);
                        int step = get_or<int>(resolve_expr(current_template.first->attribute("step").as_string("1")).value_or(1),1);
                        for(int i=from; i<to; i+=step){
                            auto frame_guard = symbols.guard();
                            if(tag!=nullptr)symbols.set(tag,i);
                            symbols.set("$",i);
                            stack_template.push({current_template.first->begin(),current_template.first->end()});
                            _parse(current_template.first);
                            //When exiting one too many is removed. restore it.
                            stack_compiled.push(current_compiled);
                        }
                    }
                    else if(strcmp(current_template.first->name()+ns_prefix_len,"for")==0){
                        const char* tag = current_template.first->attribute("tag").as_string();
                        const char* in = current_template.first->attribute("in").as_string();
                        //filter
                        //sort-by
                        //order-by
                        int limit = get_or<int>(resolve_expr(current_template.first->attribute("limit").as_string("-1")).value_or(-1),-1);
                        int offset = get_or<int>(resolve_expr(current_template.first->attribute("offset").as_string("0")).value_or(0),0);

                        bool empty=false;
                        
                        auto expr = resolve_expr(in);

                        if(empty){
                            PREFIX_STR(EMPTY_TAG,"empty");

                            for(const auto& el: current_template.first->children(EMPTY_TAG)){
                                stack_template.push({el.begin(),el.end()});
                                _parse(current_template.first);
                                stack_compiled.push(current_compiled);
                            }
                        }
                        //Only a node is acceptable in this context, otherwise show the error
                        else if(!expr.has_value() || !std::holds_alternative<const pugi::xml_node>(expr.value())){ 
                            PREFIX_STR(ERROR_TAG,"error");

                            for(const auto& el: current_template.first->children(ERROR_TAG)){
                                stack_template.push({el.begin(),el.end()});
                                _parse(current_template.first);
                                stack_compiled.push(current_compiled);
                            }
                        }
                        else{
                                //Header (once)
                                {
                                    PREFIX_STR(HEADER_TAG,"header");
                                    for(const auto& el: current_template.first->children(HEADER_TAG)){
                                        stack_template.push({el.begin(),el.end()});
                                        _parse(current_template.first);
                                        stack_compiled.push(current_compiled);
                                    }
                                }
                            
                                //Items (iterate)
                                PREFIX_STR(ITEM_TAG,"item");
                                for(auto& i : std::get<const pugi::xml_node>(expr.value()).children()){
                                    auto frame_guard = symbols.guard();
                                    if(tag!=nullptr)symbols.set(tag,i);
                                    symbols.set("$",i);
                                    for(const auto& el: current_template.first->children(ITEM_TAG)){
                                        stack_template.push({el.begin(),el.end()});
                                        _parse(current_template.first);
                                        stack_compiled.push(current_compiled);
                                    }

                                }

                                //Footer (once)
                                {
                                    PREFIX_STR(FOOTER_TAG,"footer");
                                    for(const auto& el: current_template.first->children(FOOTER_TAG)){
                                        stack_template.push({el.begin(),el.end()});
                                        _parse(current_template.first);
                                        stack_compiled.push(current_compiled);
                                    }
                                }
                        }
                    }
                    else if(strcmp(current_template.first->name()+ns_prefix_len,"for-prop")==0){}
                    else if(strcmp(current_template.first->name()+ns_prefix_len,"element")==0){
                        PREFIX_STR(TYPE_ATTR,"type");
                        //It is possible for it to generate strange results as strings are not validated by pugi
                        auto symbol = resolve_expr(current_template.first->attribute(TYPE_ATTR).as_string("$"));
                        if(!symbol.has_value()){
                        }
                        else if(std::holds_alternative<std::string>(symbol.value())){
                            auto child = current_compiled.append_child(std::get<std::string>(symbol.value()).c_str());
                            for(auto& attr : current_template.first->attributes()){
                                if(strcmp(attr.name(),TYPE_ATTR)!=0)child.append_attribute(attr.name()).set_value(attr.value());
                            }
                            stack_compiled.push(child);

                            stack_template.push({current_template.first->begin(),current_template.first->end()});
                            _parse(current_template.first);
                            stack_compiled.push(current_compiled);
                        }
                        else if(std::holds_alternative<const pugi::xml_node>(symbol.value())){
                            auto child = current_compiled.append_child(std::get<const pugi::xml_node>(symbol.value()).text().as_string());
                            for(auto& attr : current_template.first->attributes()){
                                if(strcmp(attr.name(),TYPE_ATTR)!=0)child.append_attribute(attr.name()).set_value(attr.value());
                            }
                            stack_compiled.push(child);

                            stack_template.push({current_template.first->begin(),current_template.first->end()});
                            _parse(current_template.first);
                            stack_compiled.push(current_compiled);
                        }
                        else{}
                    }
                    else if(strcmp(current_template.first->name()+ns_prefix_len,"value")==0){
                        auto symbol = resolve_expr(current_template.first->attribute("src").as_string("$"));
                        if(!symbol.has_value()){
                            /*Show default content if search fails*/
                            stack_template.push({current_template.first->begin(),current_template.first->end()});
                            _parse(current_template.first);
                            stack_compiled.push(current_compiled);
                        }
                        else{
                            if(std::holds_alternative<int>(symbol.value())){
                                current_compiled.append_child(pugi::node_pcdata).set_value(std::to_string(std::get<int>(symbol.value())).c_str());
                            }
                            else if(std::holds_alternative<const pugi::xml_attribute>(symbol.value())) {
                                current_compiled.append_child(pugi::node_pcdata).set_value(std::get<const pugi::xml_attribute>(symbol.value()).as_string());
                            }
                            else if(std::holds_alternative<std::string>(symbol.value())) {
                                current_compiled.append_child(pugi::node_pcdata).set_value(std::get<std::string>(symbol.value()).c_str());
                            }
                            else if(std::holds_alternative<const pugi::xml_node>(symbol.value())) {
                                auto tmp = std::get<const pugi::xml_node>(symbol.value());
                                current_compiled.append_copy(tmp);
                            }
                        }
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