project(
  'tcc',
  'c',
  version: '0.9.28rc-vs',
)

src = files(
  'libtcc.c',
)

include_dir = include_directories('.')

libtcc = library(
  'libtcc',
  src,
  include_directories: [include_dir],
  #install: not meson.is_subproject(),
  install: true
)

libtcc_dep = declare_dependency(
  include_directories: include_dir,
  link_with: libtcc,
)

pkg_mod = import('pkgconfig')

pkg_mod.generate(
  libtcc,
  name: 'libtcc',
  description: 'A tiny C compiler',
  libraries: libtcc,
)
