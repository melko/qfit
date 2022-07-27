cpp_args = []

if qwt_dep.found()
    qfit_ui = 'maindialog.ui'
    cpp_args += '-DWITH_QWT'
else
    qfit_ui = 'maindialog_noqwt.ui'
    warning('QFit will be built without the plot widget')
endif

qt5 = import('qt5')
pre_files = qt5.preprocess(
    moc_headers: ['qfit.h', 'about.h'],
    ui_files: qfit_ui,
    qresources: 'resources.qrc',
    dependencies: qt5_dep,
    include_directories: lib_inc,
)
qfit_exe = executable(
    'qfit',
    sources: ['main.cpp', 'qfit.cpp', 'about.cpp', pre_files],
    cpp_args: cpp_args,
    dependencies: [qt5_dep, qwt_dep],
    include_directories: lib_inc,
    link_with: fit_lib,
    install: true,
)

install_data('qfit.desktop', install_dir: 'share/applications')
install_data('qfit.png', install_dir: 'share/pixmaps')
