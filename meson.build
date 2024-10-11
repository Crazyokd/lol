project('lol', ['c'], version: '0.0.1', meson_version: '>= 0.56.0')

cc = meson.get_compiler('c')

# Enable AddressSanitizer
add_global_arguments('-fsanitize=address', language : 'c')
add_global_link_arguments('-fsanitize=address', language : 'c')

prefix = get_option('prefix')

bindir = join_paths(prefix, get_option('bindir'))
libdir = join_paths(prefix, get_option('libdir'))

libpthread_dep = cc.find_library('pthread', required : true)

lol_sources = [
    'lol.c',
]

lol_lib = library(
    'lol',
    lol_sources,
    include_directories: ['.'],
    dependencies : [libpthread_dep],
    install_dir: libdir,
    install: true,
)

# for extern use
lol_dep = declare_dependency(
    link_with : lol_lib,
    dependencies : [libpthread_dep],
    include_directories : ['.'],
)

message(
    '\n'.join(
        [
            '',
            '        prefix:                       ' + prefix,
            '        libdir:                       ' + libdir,
            '        bindir:                       ' + bindir,
            '        sysconfdir:                   ' + join_paths(
                prefix,
                get_option('sysconfdir'),
            ),
            '        localstatedir:                ' + join_paths(
                prefix,
                get_option('localstatedir'),
            ),
            '        build location:               ' + meson.current_build_dir(),
            '        source location:              ' + meson.current_source_dir(),
            '        compiler:                     ' + cc.get_id(),
            '        debugging support:            ' + get_option('buildtype'),
            '',
        ],
    ),
)

executable(
    'lol-example',
    'example.c',
    include_directories: ['.'],
    dependencies: [lol_dep],
    install_rpath: libdir,
    install_dir: bindir,
    install: true,
)

executable(
    'lol-test',
    'test.c',
    include_directories: ['.'],
    dependencies: [lol_dep, libpthread_dep],
    install_rpath: libdir,
    install_dir: bindir,
    install: true,
)

executable(
    'lol-test2',
    'test2.c',
    include_directories: ['.'],
    dependencies: [lol_dep],
    install_rpath: libdir,
    install_dir: bindir,
    install: true,
)

muon = find_program('muon', required: false) # https://github.com/annacrombie/muon
if muon.found()
    run_target(
        'muon-fmt',
        command: [
            'muon',
            'fmt',
            '-i', join_paths(meson.current_source_dir(), 'meson.build'),
            join_paths(meson.current_source_dir(), 'src', 'meson.build'),
        ],
    )
    # so how muon analyze?
endif

clangformat = find_program('clang-format', required: false)
if clangformat.found()
    run_target(
        'format',
        command: [
            'format.sh',
            meson.version().version_compare('>= 0.55.0') ? clangformat.full_path() : clangformat.path(),
            meson.current_source_dir(),
        ],
    )
endif
