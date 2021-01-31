project('z experimentum', 'c',
  version : '1.0.0',
  license : 'MPL 2.0')

add_global_arguments('-DCZMQ_BUILD_DRAFT_API', language : 'c')
zyre_dep = dependency('libzyre')
czmq_dep = dependency('libczmq')

inc = include_directories('include')

subdir('include')
subdir('src')
subdir('test')

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  libraries : zexperimentum,
  version : '1.0',
  name : 'libzexperimentum',
  filebase : 'zexperimentum',
  description : 'A library of z experiments.'
)

