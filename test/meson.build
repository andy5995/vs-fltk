test_path_resolver = executable(
    'path-resolver',
    ['./path-resolver.cpp'], 
    dependencies: [vs_fltk_dep],
    override_options : ['c_std=c11', 'cpp_std=c++20']
)

test('path resolver', test_path_resolver)