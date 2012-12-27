#ifndef JADI_H
#define JADI_H

#define JADI_LINUX 0
#define JADI_OSX 1
#define JADI_IOS 2
#define JADI_WIN 3

#ifndef JADI_PLATFORM
#error "ERROR: Define JADI_PLATFORM = {JADI_LINUX, JADI_OSX, JADI_IOS, JADI_WIN}"
#endif

#if JADI_PLATFORM == JADI_WIN
#define GLEW_STATIC
#include <Windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <jadi/GL.h>
#include <jadi/Math.h>
#include <jadi/OBJ.h>
#include <jadi/Camera.h>
#include <jadi/Font.h>

#endif
