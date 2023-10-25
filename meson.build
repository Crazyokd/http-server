project('http-server', ['c', 'cpp'],
  version: '0.0.1',
  meson_version: '>= 0.56.0'
)

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

comm_lib = meson.project_source_root() / 'lib'
cppproperties_dep = cc.find_library('cppproperties', dirs : comm_lib)

install_subdir('templates', install_dir : prefix,
  strip_directory : true # 仅复制templates下的子目录
)
install_subdir('config', install_dir : prefix)

fmt_dep = dependency('fmt', fallback : ['fmt', 'fmt_dep'])

subdir('module')
subdir('src')
subdir('test')

message('\n'.join([
  '',
  '        prefix:                       ' + prefix,
  '        libdir:                       ' + libdir,
  '        bindir:                       ' + bindir,
  '        sysconfdir:                   ' + join_paths(prefix, get_option('sysconfdir')),
  '        localstatedir:                ' + join_paths(prefix, get_option('localstatedir')),
  '        build location:               ' + meson.current_build_dir(),
  '        source location:              ' + meson.current_source_dir(),
  '        compiler:                     ' + cc.get_id(),
  '        debugging support:            ' + get_option('buildtype'),
  '',
]))

cppcheck = find_program('cppcheck', required: false)
if cppcheck.found()
    run_target('analyze-cppcheck',
    command : [ 'static-code-analyze.sh',
        meson.version().version_compare('>= 0.55.0') ?
          cppcheck.full_path() : cppcheck.path(),
        meson.current_build_dir(),
        meson.current_source_dir()
        ]
    )
endif

clangtidy = find_program('run-clang-tidy', required: false) # requires clang-tools package
if clangtidy.found() != true
    clangtidy = find_program('clang-tidy', required: false)
endif
if clangtidy.found()
    run_target('analyze-clang-tidy',
    command : [ 'static-code-analyze.sh',
        meson.version().version_compare('>= 0.55.0') ?
          clangtidy.full_path() : clangtidy.path(),
        meson.current_build_dir(),
        meson.current_source_dir()
        ]
    )
endif
