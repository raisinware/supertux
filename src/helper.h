#pragma once
#include <stdexcept>
//#include <SDL.h>

#ifdef __GNUC__
    #define SUPERTUX_DEPRECATED __attribute__((deprecated()))
#else
    #define SUPERTUX_DEPRECATED
#endif
