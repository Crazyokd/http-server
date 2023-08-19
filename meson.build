project('http-server', ['c', 'cpp'])

prefix = run_command('sh', '-c', 'echo $PWD').stdout().strip() / 'install'

bindir = join_paths(prefix, get_option('bindir'))
libdir = join_paths(prefix, get_option('libdir'))

comm_inc = include_directories('''
    include
'''.split())

install_subdir('templates', install_dir : prefix,
    strip_directory : true # 仅复制templates下的子目录
    )
subdir('module')
subdir('src')
