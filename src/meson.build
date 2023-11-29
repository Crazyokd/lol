sources = [
    'log.c',
    'lol-strings.c',
    'core.c',
]
lol = library(
    'lol',
    sources,
    include_directories: [comm_inc],
    install_dir: libdir,
    install: true,
)

# for extern use
lol_dep = declare_dependency(
    link_with : lol,
    include_directories : [comm_inc],
)
