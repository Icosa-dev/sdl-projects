project(
        'sdl-projects',
        'c',
        version: '0.1',
        default_options: ['warning_level=3'],
)

sdl3_dep = dependency('sdl3', fallback: ['sdl3', 'sdl3_dep'])
include_dir = include_directories('include')
project_deps = [
        sdl3_dep,
        declare_dependency(include_directories: include_dir)
]

subdirs = ['snake', 'sv', 'boids', 'cgl']

foreach d : subdirs
        subdir(d)
endforeach
