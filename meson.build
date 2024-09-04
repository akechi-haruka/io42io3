project('io42io3', 'c', version: '1.0')

add_project_arguments(
    '-DCOBJMACROS',
    '-DDIRECTINPUT_VERSION=0x0800',
    '-DWIN32_LEAN_AND_MEAN',
    '-D_WIN32_WINNT=_WIN32_WINNT_WIN8',
    '-DMINGW_HAS_SECURE_API=1',
    '-fdiagnostics-color=never',
    language: 'c',
)

# Use get_argument_syntax() instead once Meson 0.49.0 releases
if meson.get_compiler('c').get_id() != 'msvc'
    add_project_arguments(
        '-ffunction-sections',
        '-fdata-sections',
        '-Wno-unknown-pragmas',
        language: 'c',
    )

    add_project_link_arguments(
        '-Wl,--enable-stdcall-fixup',
        '-Wl,--exclude-all-symbols',
        '-Wl,--gc-sections',
        '-Wl,-Bstatic',
        '-static-libgcc',
        '-static',
        language: 'c',
    )
endif

cc = meson.get_compiler('c')

hidapi_dep = declare_dependency(
  dependencies : cc.find_library('hidapi', dirs : meson.current_source_dir() + '/lib/' + host_machine.cpu_family()),
  include_directories : include_directories('lib/include'))
meson.override_dependency('hidapi', hidapi_dep)

inc = include_directories('.')
ws2_lib = cc.find_library('ws2_32')

segapi = subproject('segapi')

subdir('io42io3')
