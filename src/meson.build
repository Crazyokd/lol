sources = [
    'log.c',
    'lol-strings.c',
    'core.c',
    'parser.c',
]

yaml_dep = dependency('yaml-0.1')

lol = library(
    'lol',
    sources,
    include_directories: [comm_inc],
    dependencies: yaml_dep,
    install_dir: libdir,
    install: true,
)

# for extern use
lol_dep = declare_dependency(
    link_with : lol,
    include_directories : [comm_inc],
)
