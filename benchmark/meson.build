benchmark_cases = ['test']

foreach case : benchmark_cases
    exe = executable('test_' + case, './' + case + '.cpp', dependencies: [vs_fltk_dep])
    benchmark('benchmark_' + case, exe)
endforeach