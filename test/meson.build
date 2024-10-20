vg = find_program('valgrind', required: false)
if vg.found()
    add_test_setup(
        'valgrind',
        exe_wrapper: [join_paths(meson.current_source_dir(), 'valgrind_wrap.sh')],
    )
endif

to_pass = [
    'tcc-integration',
    'quickjs-integration',
    'static-xml',
]
to_fail = ['path-resolver']

foreach case : to_pass
    exe = executable(
        'test_' + case,
        './' + case + '.cpp',
        dependencies: [vs_fltk_dep],
        cpp_args: '-DIS_TESTING=true',
    )
    test('test_' + case, exe, env: ['VS_HEADLESS=true'])
endforeach

foreach case : to_fail
    exe = executable(
        'test_' + case,
        './' + case + '.cpp',
        dependencies: [vs_fltk_dep],
        cpp_args: '-DIS_TESTING=true',
    )
    test('test_' + case, exe, should_fail: true, env: ['VS_HEADLESS=true'])
endforeach