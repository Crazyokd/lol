project('lol', ['c'], version: '0.0.1', meson_version: '>= 0.56.0')

cc = meson.get_compiler('c')

prefix = get_option('prefix')

bindir = join_paths(prefix, get_option('bindir'))
libdir = join_paths(prefix, get_option('libdir'))

comm_inc = include_directories(
    '''
  include
'''.split(),
)


subdir('src')

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
    include_directories: [comm_inc],
    link_with: [lol],
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
