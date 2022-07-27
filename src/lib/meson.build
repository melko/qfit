lib_version_array = meson.project_version().split('.')
configure_file(
    input: 'fitconfig.h.in',
    output: 'fitconfig.h',
    format: 'cmake',
    configuration: {
        'PROJECT_VERSION_MAJOR': lib_version_array[0],
        'PROJECT_VERSION_MINOR': lib_version_array[1],
        'PROJECT_VERSION_PATCH': lib_version_array[2],
    },
)
lib_inc = include_directories('.')
fit_lib = static_library('fit', 'libfile.cpp', 'log.cpp', 'fittools.cpp')
