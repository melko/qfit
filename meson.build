project('qfit', 'cpp', version: '0.4.0', license: 'GPL3')

qt5_dep = dependency(
    'qt5',
    modules: ['Widgets'],
    required: get_option('WITH_GUI'),
)

qwt_dep = dependency(
    'Qt5Qwt6',
    version: '>=6',
    required: get_option('WITH_QWT'),
)

subdir('src')
