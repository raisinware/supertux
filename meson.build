project('supertux-milestone1', 'cpp',
        default_options: 'default_library=static')

sdl1 = dependency('sdl12_compat')

# TODO: add sound + opengl support
add_project_arguments('-DNOSOUND', language: ['cpp'])
add_project_arguments('-DNOOPENGL', language: ['cpp'])

subdir('src')
