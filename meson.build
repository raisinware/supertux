project('supertux-milestone1', 'cpp',
        version: '0.1.4')

sdl2 = dependency('sdl2')
sdl2_image = dependency('SDL2_image')
zlib = dependency('zlib')
enable_sound = get_option('enable_sound')

if enable_sound
    sdl2_mixer = dependency('SDL2_mixer')
else
    sdl2_mixer = dependency('SDL2_mixer', optional: true)
    add_project_arguments('-DNOSOUND', language: ['cpp'])
endif

data_prefix = './data'
#data_prefix = get_option('prefix') / get_option('datadir') / 'supertux-milestone1'
message('Data Prefix: ' + data_prefix)

add_project_arguments('-DDATA_PREFIX="@0@"'.format(data_prefix), language: ['cpp'])
add_project_arguments('-DVERSION="@0@"'.format(meson.project_version()), language: ['cpp'])

subdir('src')
