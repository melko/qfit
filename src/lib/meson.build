lib_version_array = meson.project_version().split('.')
configure_file(
    output: 'fitconfig.h',
    configuration: {
        'QFIT_VERSION_MAJOR': lib_version_array[0],
        'QFIT_VERSION_MINOR': lib_version_array[1],
        'QFIT_VERSION_PATCH': lib_version_array[2],
        'WITH_GUI': qt5_dep.found(),
        'WITH_QWT': qwt_dep.found(),
    },
)
lib_inc = include_directories('.')
fit_lib = static_library('fit', 'libfile.cpp', 'log.cpp', 'fittools.cpp')
