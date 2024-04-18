project('supertux-milestone1', 'cpp',
        version: '0.1.4')

sdl1 = dependency('sdl12_compat')
sdl1_image = dependency('SDL_image')
zlib = dependency('zlib')

#data_prefix = './data'
data_prefix = get_option('prefix') / get_option('datadir') / 'supertux-milestone1'
message('Data Prefix: ' + data_prefix)

add_project_arguments('-DDATA_PREFIX="@0@"'.format(data_prefix), language: ['cpp'])
add_project_arguments('-DVERSION="@0@"'.format(meson.project_version()), language: ['cpp'])
# TODO: add sound + opengl support
add_project_arguments('-DNOSOUND', language: ['cpp'])
add_project_arguments('-DNOOPENGL', language: ['cpp'])

subdir('src')
