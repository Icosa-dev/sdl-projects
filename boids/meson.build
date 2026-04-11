program = 'boids'

sources = [
        'main.c',
        'boid.c'
]

# boids need the math library
cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required: true)
boids_deps = project_deps + [m_dep] 

executable(program, sources,
        dependencies: boids_deps,
        install: true
)
