#ifndef JADI_UTILS_H
#define JADI_UTILS_H

#include <assert.h>
#include <cstddef>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <float.h>
#include <jadi/OBJ.h>


#if JADI_PLATFORM == JADI_OSX
#include <libgen.h> /* dirname */
#include <CoreFoundation/CFRunLoop.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <mach-o/dyld.h> /* _NSGetExecutablePath */
#include <sys/resource.h>
#include <sys/sysctl.h>
#include <unistd.h>  /* sysconf */
#endif


#ifndef PI
#define PI 3.14159265358979323846
#endif

#ifndef TWO_PI
#define TWO_PI 6.28318530717958647693
#endif

#ifndef M_TWO_PI
#define M_TWO_PI 6.28318530717958647693
#endif

#ifndef FOUR_PI
#define FOUR_PI 12.56637061435917295385
#endif

#ifndef HALF_PI
#define HALF_PI 1.57079632679489661923
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD (PI/180.0)
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG (180.0/PI)
#endif

#ifndef MIN
#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef CLAMP
#define CLAMP(val,min,max) (MAX(MIN(val,max),min))
#endif

#ifndef ABS
#define ABS(x) (((x) < 0) ? -(x) : (x))
#endif

#ifndef DX
#define DX(i,j,w)((j)*(w))+(i)
#endif

#define GLSL(version, shader)  "#version " #version "\n" #shader


#define eglGetError( )                                                  \
  {                                                                     \
    for ( GLenum Error = glGetError( ); ( GL_NO_ERROR != Error ); Error = glGetError( ) ) \
      {                                                                 \
        switch ( Error )                                                \
          {                                                             \
            case GL_INVALID_ENUM:      printf( "\n%s\n\n", "GL_INVALID_ENUM"      ); assert( 0 ); break; \
            case GL_INVALID_VALUE:     printf( "\n%s\n\n", "GL_INVALID_VALUE"     ); assert( 0 ); break; \
            case GL_INVALID_OPERATION: printf( "\n%s\n\n", "GL_INVALID_OPERATION" ); assert( 0 ); break; \
            case GL_STACK_OVERFLOW:    printf( "\n%s\n\n", "GL_STACK_OVERFLOW"    ); assert( 0 ); break; \
            case GL_STACK_UNDERFLOW:   printf( "\n%s\n\n", "GL_STACK_UNDERFLOW"   ); assert( 0 ); break; \
            case GL_OUT_OF_MEMORY:     printf( "\n%s\n\n", "GL_OUT_OF_MEMORY"     ); assert( 0 ); break; \
            default:                                                                              break; \
          }                                                             \
      }                                                                 \
  }

#define eglCheckFramebufferStatus( )                                    \
  {                                                                     \
    switch ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) )               \
      {                                                                 \
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:         printf( "\n%s\n\n", "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"         ); assert( 0 ); break; \
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: printf( "\n%s\n\n", "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" ); assert( 0 ); break; \
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:        printf( "\n%s\n\n", "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"        ); assert( 0 ); break; \
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:        printf( "\n%s\n\n", "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"        ); assert( 0 ); break; \
        case GL_FRAMEBUFFER_UNSUPPORTED:                   printf( "\n%s\n\n", "GL_FRAMEBUFFER_UNSUPPORTED"                   ); assert( 0 ); break; \
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:        printf( "\n%s\n\n", "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"        ); assert( 0 ); break; \
        case GL_FRAMEBUFFER_UNDEFINED:                     printf( "\n%s\n\n", "GL_FRAMEBUFFER_UNDEFINED"                     ); assert( 0 ); break; \
        default:                                                                                                                              break; \
      }                                                                 \
  }

#define eglGetShaderInfoLog( Shader )                           \
  {                                                             \
    GLint   Status, Count;                                      \
    GLchar *Error;                                              \
                                                                \
    glGetShaderiv( Shader, GL_COMPILE_STATUS, &Status );        \
                                                                \
    if ( !Status )                                              \
      {                                                         \
        glGetShaderiv( Shader, GL_INFO_LOG_LENGTH, &Count );    \
                                                                \
        if ( Count > 0 )                                        \
          {                                                     \
            Error = (GLchar *)malloc(Count);                    \
            glGetShaderInfoLog( Shader, Count, NULL, Error );   \
                                                                \
            printf( "%s\n", Error );                            \
                                                                \
            free( Error );                                      \
                                                                \
            assert( 0 );                                        \
          }                                                     \
      }                                                         \
  }

// as described in: "From Quaternion to Matrix and Back", J.M.P. van Waveren, 27th feb. 2005, id software
static float fast_sqrt(float x) {
  long i; 
  float y, r; 
  y = x * 0.5f; 
  i = *(long *)( &x ); 
  i = 0x5f3759df - ( i >> 1 ); 
  r = *(float *)( &i ); 
  r = r * ( 1.5f - r * r * y ); 
  return r; 
}

static std::string get_data_path() {
}


#if JADI_PLATFORM == JADI_OSX
static std::string get_exe_path() {
  char buffer[1024];
  uint32_t usize = sizeof(buffer);;

  int result;
  char* path;
  char* fullpath;

  result = _NSGetExecutablePath(buffer, &usize);
  if (result) {
    return "";
  }

  path = (char*)malloc(2 * PATH_MAX);
  fullpath = realpath(buffer, path);

  if (fullpath == NULL) {
    free(path);
    return "";
  }

  strncpy(buffer, fullpath, usize);
  free(fullpath);
  const char* dn = dirname(buffer);
  usize = strlen(dn);
  std::string ret(dn, usize) ;
  ret.push_back('/');
  return ret;
}

static std::string to_data_path(const std::string filename) {
  std::string exepath = get_exe_path();
  exepath += "data/" +filename;
  return exepath;
}


static uint64_t millis(void) {
  mach_timebase_info_data_t info;
  if (mach_timebase_info(&info) != KERN_SUCCESS) {
    abort();
  }
  return (mach_absolute_time() * info.numer / info.denom) / 1000000;
}



#endif

// Simple wrapper which loads an object file from the data path, returns number of vertices
static size_t load_obj_file(const std::string& filename, float** result, bool useNormals) {
  OBJ obj;
  if(!obj.load(to_data_path(filename))) {
    *result = NULL;
    return 0;
  }
  return obj.getVertices(result, useNormals);
}

static float random(float max) {
  return max * rand() / (RAND_MAX + 1.0f);
}

static float random(float x, float y) {
  float high = 0;
  float low = 0;
  float result = 0;
	
  high = std::max<float>(x,y);
  low = std::min<float>(x,y);
  result = low + ((high-low) * rand()/(RAND_MAX + 1.0));
  return result;
}

#endif 
