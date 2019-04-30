project('qfit', 'cpp', version: '0.3.1', license: 'GPL3')

qt5_dep = dependency(
    'qt5',
    modules: ['Widgets'],
    required: get_option('WITH_GUI'),
)

qwt_dep = dependency(
    'Qwt',
    version: '>=6',
    method: 'cmake',
    cmake_module_path: 'CMakeModules',
    required: get_option('WITH_QWT'),
)

subdir('src')
