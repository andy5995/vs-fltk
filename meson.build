project(
    'vs-fltk',
    ['c', 'cpp', 'swift'],
    default_options: ['c_std=gnu23', 'cpp_std=c++23'],
)


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

hashlib_proj = subproject('hashlib')
hashlib_dep = hashlib_proj.get_variable('hashlib_dep')

pugixml_proj = subproject('pugixml')
pugixml_dep = pugixml_proj.get_variable('pugixml_dep')

treesitter_proj = subproject('tree-sitter')
treesitter_dep = treesitter_proj.get_variable('tree_sitter_dep')

libtcc_proj = subproject('libtcc')
libtcc_dep = libtcc_proj.get_variable('libtcc_dep')

uv_proj = subproject('libuv')
uv_dep = uv_proj.get_variable('libuv_dep')

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
    treesitter_dep,
    libtcc_dep,
    quickjs_dep,
    sqlite_dep,
    sqlitecpp_dep,
    uv_dep,
    hashlib_dep,
    md4c_dep,
]

vs_fltk_flags = []

if curl.found()
    vs_fltk_deps += [curl]
    vs_fltk_flags += '-DHAS_CURL'
else
endif

subdir('./src/components/autogen')

vs_fltk = shared_library(
    'vs-fltk',
    [
        './src/cache/memory-storage.cpp',

        './src/utils/paths.cpp',
        './src/utils/strings.cpp',
        './src/utils/env.cpp',
        './src/utils/policies.cpp',
        './src/utils/policies.internal.cpp',

        './src/tcc-wrap.cpp',

        './src/resolvers/fs.cpp',
        './src/resolvers/network.cpp',

        './src/loader.cpp',

        #TODO: move them in a split meson file
        './src/components/containers.cpp',
        './src/components/markdown-view.cpp',
        './src/components/code-editor.cpp',
        './src/components/data.xml.cpp',
        './src/components/data.json.cpp',
        './src/components/data.sqlite.cpp',
        './src/components/data.csv.cpp',

        autogen_components,

        './src/ui-frame.cpp',
        './src/ui.cpp',
        './src/ui-base.cpp',
        './src/ui.template-builders.cpp', #TODO: Deprecate
        './src/ui.props-dispatchers.cpp', #TODO: Deprecate

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
)

vs_fltk_dep = declare_dependency(
    include_directories: include_dirs,
    link_with: vs_fltk,
)

cello_project = subproject('cello')
cello_dep = cello_project.get_variable('cello_dep')

shared_library('vs-cello', dependencies: [cello_dep])

executable(
    'vs',
    [
        './src/app/main.cpp',
        './src/app/settings.cpp',
        './src/app/updater.cpp',
    ],
    dependencies: [vs_fltk_dep],
    override_options: ['c_std=gnuc23', 'cpp_std=c++23'],
)

executable(
    'swift_vs',
    [
        './src/swift-app/app.swift',
    ],
    dependencies: [vs_fltk_dep],
)

subdir(['./components/'])
subdir(['./experiments/'])
subdir(['./test/'])
subdir(['./benchmark/'])