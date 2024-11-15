const elements=[
    'app',
    'component',
    'static-data',
    'import',
    'use',
    'viewport',
    'data.*',
    'script',
    'mixin',
    'debug',
    'namespace'
]

//Not sure about these
const attributes=[
    'src.*',
    'name',
    'frame.*',
    'template',
    'mixins'
]

function toSymbol(str:string){
    return str.toUpperCase().replaceAll('.','_').replaceAll('-','_');
}

export const ns_strings_struct =
`struct ns_strings{
    private:
        char* data = nullptr;
    
    public:
        //TAGS
${elements.map(x=>
`        const char *${toSymbol(x)}_TAG;`
).join('\n')
}

        //PROPS
${attributes.map(x=>
`        const char *${toSymbol(x)}_PROP;`
).join('\n')
}

        void prepare(const char * ns_prefix);
        inline ~ns_strings(){if(data!=nullptr)delete[] data;}
};`

//TODO: Write code to write strings

export const ns_strings_impl =
`
void preprocessor::ns_strings::prepare(const char * ns_prefix){
#   define WRITE(name,value) name=data+count;memcpy(data+count,ns_prefix,ns_prefix_len);memcpy(data+count+ns_prefix_len,value,std::char_traits<char>::length(value));data[count+ns_prefix_len+std::char_traits<char>::length(value)]=0;count+=ns_prefix_len+std::char_traits<char>::length(value)+1;
#   define STRLEN(str) ns_prefix_len+std::char_traits<char>::length(str)+1

    size_t ns_prefix_len=strlen(ns_prefix);

    if(data!=nullptr)delete []data;
    data = new char[]

#   undef WRITE
#   undef STRLEN
}
`


//Generate ui-tree.xml.ns.autofrag.hpp and ui-tree.xml.ns.autofrag.cpp