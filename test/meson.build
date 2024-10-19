test_path_resolver = executable(
    'path-resolver',
    ['./path-resolver.cpp'],
    dependencies: [vs_fltk_dep],
)

test_quickjs_integration = executable(
    'quickjs-integration',
    ['./quickjs-integration.cpp'],
    dependencies: [vs_fltk_dep],
)

test_static_xml = executable(
    'static-xml',
    ['./static-xml.cpp'],
    dependencies: [vs_fltk_dep],
)

test_tcc_integration = executable(
    'tcc-integration',
    ['./tcc-integration.cpp'],
    dependencies: [vs_fltk_dep],
)

test('path resolver', test_path_resolver)
test('static xml', test_static_xml)

test('tcc_integration', test_tcc_integration)
test('quickjs integration', test_quickjs_integration)