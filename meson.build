project('http-server', ['c', 'cpp'])

add_project_arguments(
  '-xc++',
  language : 'c'
)
cc = meson.get_compiler('cpp')

prefix = run_command('sh', '-c', 'echo $PWD').stdout().strip() / 'install'

bindir = join_paths(prefix, get_option('bindir'))
libdir = join_paths(prefix, get_option('libdir'))

comm_inc = include_directories('''
    include
'''.split())

comm_lib = meson.source_root() / 'lib'
cppproperties_dep = cc.find_library('cppproperties', dirs : comm_lib)

install_subdir('templates', install_dir : prefix,
    strip_directory : true # 仅复制templates下的子目录
    )
install_subdir('config', install_dir : prefix)

fmt_dep = dependency('fmt', fallback : ['fmt', 'fmt_dep'])

subdir('module')
subdir('src')
subdir('test')