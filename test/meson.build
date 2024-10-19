vg = find_program('valgrind', required: false)
if vg.found()
    add_test_setup(
        'valgrind',
        exe_wrapper: [join_paths(meson.current_source_dir(), 'valgrind_wrap.sh')],
    )
endif

test_cases = ['tcc-integration', 'quickjs-integration', 'static-xml', 'path-resolver']

foreach case : test_cases
    exe = executable('test_' + case, './' + case + '.cpp', dependencies: [vs_fltk_dep])
    test('test_' + case, exe)
endforeach