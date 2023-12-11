lol_sysconfdir = join_paths(sysconfdir, 'lol')
meson.add_install_script(python3_exe, '-c',
        mkdir_p.format(lol_sysconfdir)) # format with @0@

conf_data = configuration_data()
        
lol_confs = '''
    lol.yaml
'''.split()

foreach file : lol_confs
    gen = configure_file(
            input : file + '.in',
            output : file,
            configuration : conf_data)
    meson.add_install_script(python3_exe, '-c',
            install_conf.format(gen, lol_sysconfdir))
endforeach