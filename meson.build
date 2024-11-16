project(
    'vs-fltk',
    ['c', 'cpp', 'swift'],
    default_options: ['c_std=gnu23', 'cpp_std=gnu++23'],
)

#Used by pugi to enable compact mode. I might want to disable execptions too.
add_global_arguments(['-DPUGIXML_COMPACT'], language : ['cpp','c'])

cmake = import('cmake')

opt_fltk = cmake.subproject_options()
opt_fltk.add_cmake_defines(
    {
        'FLTK_BUILD_FLUID': false,
        'FLTK_BUILD_EXAMPLES': false,
        'FLTK_BUILD_TEST': false,
        'FLTK_BUILD_OPTIONS': false,
        'FLTK_BUILD_SHARED_LIBS': true,
        'FLTK_BACKEND_X11': true,
        'CMAKE_POSITION_INDEPENDENT_CODE': true,
    },
)

libfltk_proj = cmake.subproject('libfltk', options: opt_fltk)
libfltk_dep = libfltk_proj.dependency(['fltk-shared'])
libfltk_images_dep = libfltk_proj.dependency(['fltk-images-shared'])
libfltk_forms_dep = libfltk_proj.dependency(['fltk-forms-shared'])
#libfltk_gl_dep = libfltk_proj.dependency(['fltk-gl'])

opt_wamr = cmake.subproject_options()
opt_wamr.add_cmake_defines(
    {
        'WAMR_BUILD_INTERP': 1,
        'WAMR_BUILD_PLATFORM': 'linux', #TODO: Make it generic based on target arch
    },
)

wamr_proj = cmake.subproject('wamr', options: opt_wamr)
wamr_dep = wamr_proj.dependency('wamr')

opt_quickjs = cmake.subproject_options()
opt_quickjs.add_cmake_defines(
    {
        'CMAKE_POSITION_INDEPENDENT_CODE': true,
    },
)

quickjs_proj = cmake.subproject('quickjs', options: opt_quickjs)
quickjs_dep = quickjs_proj.dependency('qjs')

md4c_proj = cmake.subproject('md4c')
md4c_dep = md4c_proj.dependency('md4c')

opt_fmt = cmake.subproject_options()
opt_fmt.add_cmake_defines(
    {
        'CMAKE_POSITION_INDEPENDENT_CODE': true,
        'BUILD_SHARED_LIBS': false
    },
)
fmt_proj = cmake.subproject('fmt', options: opt_fmt)
fmt_dep = fmt_proj.dependency('fmt')

hashlib_proj = subproject('hashlib')
hashlib_dep = hashlib_proj.get_variable('hashlib_dep')

json_proj = subproject('nlohmann_json')
json_dep = json_proj.get_variable('nlohmann_json_dep')

pugixml_proj = subproject('pugixml')
pugixml_dep = pugixml_proj.get_variable('pugixml_dep')

vs_templ_proj = subproject('vs-templ')
vs_templ_dep = vs_templ_proj.get_variable('vs_templ_dep')


treesitter_proj = subproject('tree-sitter')
treesitter_dep = treesitter_proj.get_variable('tree_sitter_dep')

libtcc_proj = subproject('libtcc')
libtcc_dep = libtcc_proj.get_variable('libtcc_dep')

#uv_proj = subproject('libuv')
#uv_dep = uv_proj.get_variable('libuv_dep')

mio_proj = cmake.subproject('mio')
mio_dep = mio_proj.dependency('mio')

#TODO convert to internald dep
sqlitecpp_dep = subproject('sqlitecpp').get_variable('sqlitecpp_dep')

sqlite_dep = subproject('sqlite3').get_variable('sqlite3_dep')

#TODO handle the case in which libcurl does not exist. Missing is fine, just no https/http prefix for paths.
curl = dependency('libcurl', required: false)

include_dirs = include_directories(['./include'])

# Prepare `vs`

vs_fltk_deps = [
    libfltk_dep,
    libfltk_images_dep,
    libfltk_forms_dep,

    pugixml_dep,
    json_dep,
    vs_templ_dep,
    fmt_dep,
    mio_dep,
    #uv_dep,    The one provided with the system if present should be fine for our tasks
    hashlib_dep,

    libtcc_dep,
    quickjs_dep,
    sqlite_dep,
    sqlitecpp_dep,

    md4c_dep,
    treesitter_dep,
]

vs_fltk_flags = ['-Wno-c23-extensions', '-Wno-vla-cxx-extension', '-Wno-unused-variable']

if curl.found()
    vs_fltk_deps += [curl]
    vs_fltk_flags += '-DHAS_CURL'
else
endif

vs_fltk_flags += '-DPUGIXML_COMPACT'

subdir('./src/components/autogen')

vs_fltk = shared_library(
    'vs-fltk',
    [
        './src/cache/memory-storage.cpp',
        './src/cache/sqlite-cache.cpp',
        './src/cache/sqlite-persistent.cpp',

        './src/utils/paths.cpp',
        './src/utils/strings.cpp',
        './src/utils/env.cpp',
        './src/utils/policies.cpp',
        './src/utils/policies.internal.cpp',
        './src/utils/tcc-wrap.cpp',

        './src/fetcher.cpp',

        './src/loader.cpp',

        #TODO: move them in a split meson file
        './src/components/containers.cpp',
        './src/components/markdown-view.cpp',
        './src/components/code-editor.cpp',
        './src/components/data.xml.cpp',
        './src/components/data.json.cpp',
        './src/components/data.sqlite.cpp',
        './src/components/data.csv.cpp',
        './src/components/data.fs.cpp',

        autogen_components,

        './src/ui-frame.cpp',
        './src/ui.cpp',
        './src/ui-base.cpp',

        './src/xml2native.cpp',

        './src/ui-tree.cpp',
        './src/ui-tree.xml.cpp',
        './src/ui-tree.symbols.cpp',

        './src/pipelines/tcc-c.cpp',
        './src/pipelines/tcc-cello.cpp',
        './src/pipelines/quickjs-js.cpp',
        './src/pipelines/wasm-wat.cpp',
        './src/pipelines/lua-lua.cpp',

        './src/themes/default-cute.cpp',

        './src/cbindings/vs.cpp',
        './src/cbindings/components.autogen.cpp',

        './src/globals.cpp'
    ],
    dependencies: vs_fltk_deps,
    include_directories: include_dirs,
    override_options: ['c_std=gnu23', 'cpp_std=gnu++23'],
    cpp_args: vs_fltk_flags,
    c_args: vs_fltk_flags,
    install: true
)

vs_fltk_dep = declare_dependency(
    include_directories: include_dirs,
    link_with: vs_fltk,
)

cello_project = subproject('cello')
cello_dep = cello_project.get_variable('cello_dep')

shared_library('vs-cello', dependencies: [cello_dep])

#TODO add also a vs_codegen to introduce that step within meson instead of keeping it separate.
#I need to run it not from the builddir but from the root.
#vs_codegen_cmd = find_program('./scripts/codegen/index.ts')
#vs_codegen = custom_target(command: 'vs-codegen', output: 'vs-codegen', command: [vs_codegen_cmd], depends:[])

vs_commons_cmd = find_program('./scripts/gen-commons.sh')
vs_commons = custom_target('vs-commons', output: 'commons', command: [vs_commons_cmd], depends:[])

executable(
    'vs',
    [
        './src/app/main.cpp',
        './src/app/settings.cpp',
        './src/app/updater.cpp',
    ],
    dependencies: [vs_fltk_dep],
    link_depends: [vs_commons],
    override_options: ['c_std=gnuc23', 'cpp_std=c++23'],
    install: true,
    #install_rpath: ''
)

executable(
    'swift_vs',
    [
        './src/swift-app/app.swift',
    ],
    dependencies: [vs_fltk_dep],
)

subdir(['./experiments/'])
subdir(['./test/'])
subdir(['./benchmark/'])

# Install stuff

install_headers(['./include/cbindings/vs.h','./include/cbindings/components.autogen.h','./include/cbindings/module.modulemap'], preserve_path: false, subdir: 'vs-fltk')

install_subdir('commons', exclude_files:['.gitignore'], install_dir:'share/vs-fltk',strip_directory:true)
install_subdir('docs',install_dir:'share/vs-fltk')
install_subdir('examples',install_dir:'share/vs-fltk')
install_subdir('bindings',install_dir:'share/vs-fltk')
install_data(['./LICENCE.md','./README.md', './RELEASE.md'],install_dir:'share/vs-fltk/')

#install_man()

pconf = import('pkgconfig')
pconf.generate(
  vs_fltk,
  description: 'VS fltk library (C interface only)',
  url: 'https://pugixml.org/',
)