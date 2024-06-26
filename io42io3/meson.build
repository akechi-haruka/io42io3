io42io3 = shared_library(
    'io42io3',
    name_prefix : '',
    include_directories : inc,
    implicit_include_directories : false,
    c_pch : '../precompiled.h',
    vs_module_defs : 'exports.def',
    link_with : [
    ],
    dependencies : [
        hidapi_dep
    ],
    sources : [
        'io42io3.c',
        'io42io3.h',
        'config.c',
        'config.h',
        'io4.c',
        'io4.h',
        'games/kca.c',
        'games/kca.h',
        'util/dprintf.c',
        'util/dprintf.h',
    ],
)
