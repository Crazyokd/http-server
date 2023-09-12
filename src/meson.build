executable('http-server', 'main.cpp',
    include_directories : [comm_inc],
    link_with : [cJSON, dl1],
    dependencies: [cppproperties_dep],
    install_rpath: libdir, # set ld_library_path
    install_dir : bindir,
    install : true)