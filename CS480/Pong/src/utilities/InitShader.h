/** @file  InitShader.h
  * @brief Header file containing the InitShader function
  */

#ifndef __OPEN_GL_SHADER_INIT__
#define __OPEN_GL_SHADER_INIT__

#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else 
#  include <GL/glew.h>
#  include <GL/glut.h>
#endif

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
#define Print(x)  std::cerr << #x " = " << (x) << std::endl

/** @brief Load vertex and fragment shader files into GPU. */
GLuint InitShader(
  const char* vShader, /**< [in] vertex shader filename. */
  const char* fShader  /**< [in] fragment shader filename. */
);

#endif // __OPEN_GL_SHADER_INIT__
