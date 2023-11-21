project('lol', ['c'], version: '0.0.1', meson_version: '>= 0.56.0')

cc = meson.get_compiler('c')

prefix = (
    run_command('sh', '-c', 'echo $PWD', check: false).stdout().strip() / 'install'
)

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

# try muon
# muon analyze
# muon fmt