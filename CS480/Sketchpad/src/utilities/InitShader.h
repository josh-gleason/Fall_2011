/** @file  InitShader.h
  * @brief Header file containing the InitShader function
  */

#ifndef __OPEN_GL_SHADER_INIT__
#define __OPEN_GL_SHADER_INIT__

#include <cmath>
#include <iostream>
#include <cstdlib>

#ifdef __APPLE__
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else 
#  include <GL/glew.h>
#  include <GL/glut.h>
#endif

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
#define Print(x)  std::cerr << #x " = " << (x) << std::endl

#ifndef NDEBUG
#   define ASSERT(condition, message) \
  do { \
    if (! (condition)) { \
      std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                << " line " << __LINE__ << ": " << message << std::endl; \
      std::exit(EXIT_FAILURE); \
    } \
  } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif


/** @brief Load vertex and fragment shader files into GPU. */
GLuint InitShader(
  const char* vShader, /**< [in] vertex shader filename. */
  const char* fShader  /**< [in] fragment shader filename. */
);

void printColMajorMatArrayf(GLfloat mat[16]);

#endif // __OPEN_GL_SHADER_INIT__
