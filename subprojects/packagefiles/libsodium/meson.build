project(
    'libsodium',
    'c',
    meson_version: '>=0.56.0',
    version: '1.0.20'
)

mod = import('unstable-external_project')

p = mod.add_project(
    'configure',
    configure_options: [
        '--prefix=@PREFIX@',
        '--libdir=@PREFIX@/@LIBDIR@',
        '--includedir=@PREFIX@/@INCLUDEDIR@',
        #'--enable-foo',
    ],
)

libsodium_dep = p.dependency('libsodium')
meson.override_dependency('libsodium', libsodium_dep)