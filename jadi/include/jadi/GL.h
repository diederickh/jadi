#ifndef JADI_GL_H
#define JADI_GL_H

#define GLSL(version, shader)  "#version " #version "\n" #shader
#include <assert.h>
#include <opengl/glew.h>
#include <opengl/glfw3.h>

#if JADI_PLATFORM == JADI_OSX
//#include <openGL/GL.h> 
//#include <openGL/glext.h>
#undef glGenVertexArrays
#undef glBindVertexArray
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#endif



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


// Creates a vertex + frag shader and a program. 
// We do not yet link the program so you can set attribute locations
inline GLuint create_shader(const char* vs, const char* fs) {
  GLuint vert_id = glCreateShader(GL_VERTEX_SHADER);
  GLuint frag_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vert_id, 1, &vs, NULL);
  glShaderSource(frag_id, 1, &fs, NULL);
  glCompileShader(vert_id); eglGetShaderInfoLog(vert_id);
  glCompileShader(frag_id); eglGetShaderInfoLog(frag_id);
  GLuint prog = glCreateProgram();
  glAttachShader(prog, vert_id);
  glAttachShader(prog, frag_id);
  return prog;
}


#endif
