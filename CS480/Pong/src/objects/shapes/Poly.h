/** @file    Poly.h
 *  @brief   Defines the abstract Poly class.
 *  @details The Poly class is used as a base class for all types of shapes.
 *           This class should be templated on a vec2, vec3, or vec4.
 */

#ifndef __JDG_POLY_H__
#define __JDG_POLY_H__

#include "vec.h"
#include "mat.h"
#include "InitShader.h"

using namespace std;

/** @brief   Abstract Poly class.
 *  @details Used as a base class for all types of shapes. This class should be
 *           templated on a vec2, vec3, or vec4.
 */
template <class Vec>
class Poly
{
public:
  /** @brief Initialize the vertex array to NULL. */ 
  Poly();

  /** @brief Returns the number of vertices in the polygon. */
  virtual int vertex_count() const = 0;

  /** @brief Returns the number of dimensions of each vertex, either 2,3 or 4. */
  virtual int vertex_dim() const = 0;

  /** @brief Returns the mode of glDrawArrays.  May be GL_TRIANGLES,
    *        GL_QUADS, etc... */
  virtual GLenum get_mode() const = 0;

  /** @brief Accessor to the vertices array. */
  virtual const Vec* getVertices() const;

  /** @brief Accessor to the colors array. */
  virtual const vec4* getColors() const;

  /** @brief Draw object to screen (should be used during display callback) */
  virtual void draw( const mat4& projection ) const;

  /** @brief Init function meant to be called during graphics initialization */
  virtual void init();

  GLuint getVao() const;
  GLuint getVPosition() const;
  GLuint getVColor() const;
protected:
  Vec* vertices; ///< Array of vertices for the object.
  vec4* colors;  ///< Array of colors, one for each vertex.
  GLuint vao;    ///< vertex array object.
  GLuint vPosition;
  GLuint vColor;

  GLuint projection;
  GLuint translation;

  GLenum mode;
};

// Method implementations

template <class Vec>
Poly<Vec>::Poly() : vertices(NULL), colors(NULL) {}

template <class Vec>
const Vec* Poly<Vec>::getVertices() const
{
  return vertices;
}

template <class Vec>
const vec4* Poly<Vec>::getColors() const
{
  return colors;
}

template <class Vec>
void Poly<Vec>::draw( const mat4& projection_mat ) const
{
  glBindVertexArray(vao);

  glUniformMatrix4fv( projection, 1, GL_TRUE, projection_mat ); 
  glDrawArrays(get_mode(), 0, vertex_count());
  glBindVertexArray(0);
}

template <class Vec>
void Poly<Vec>::init()
{
  int vertices_length = sizeof(Vec)*vertex_count();
  int colors_length = sizeof(vec4)*vertex_count();
  int total_length = vertices_length+colors_length;

  // load the shaders
  GLuint program = InitShader("../src/shaders/vshader.glsl",
                              "../src/shaders/fshader.glsl");
  glUseProgram(program);

  // create vertex array for the polygon
  glGenVertexArrays(1,&vao);
  glBindVertexArray(vao);
  // create a buffer object for the polygon
  GLuint buffer;
  glGenBuffers(1,&buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, total_length, NULL, GL_STATIC_DRAW);

  // insert the vertices
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_length, vertices);
  // insert the colors
  glBufferSubData(GL_ARRAY_BUFFER, vertices_length, colors_length, colors);

  // set vertices to vPosition
  GLuint vPosition;
  vPosition = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, vertex_dim(), GL_FLOAT, GL_FALSE, 0,
      BUFFER_OFFSET(0));

  GLuint vColor;
  vColor = glGetAttribLocation(program, "vColor");
  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
      BUFFER_OFFSET(vertices_length));

  projection = glGetUniformLocation(program,"projection");
  //model_view  = glGetUniformLocation(program,"model_view");
  //translation = glGetUniformLocation(program,"translation");
}

template <class Vec>
GLuint Poly<Vec>::getVao() const
{
  return vao;
}

template <class Vec>
GLuint Poly<Vec>::getVPosition() const
{
  return vPosition;
}

template <class Vec>
GLuint Poly<Vec>::getVColor() const
{
  return vColor;
}
#endif // __JDG_POLY_H__
