#include <GL/glew.h>
#include <GL/glut.h>

#include "Shape.h"
#include "InitShader.h"

Shape::Shape() :
  m_vertices(NULL),
  m_vertex_count(0)
{}

Shape::Shape(int numVertecies, const vec4* vertices, const vec4& color,
  const vec2& center) :
  m_vertices(new vec4[numVertecies]),
  m_vertex_count(numVertecies)
{
  if ( vertices != NULL )
    for ( int i = 0; i < numVertecies; ++i )
      m_vertices[i] = vertices[i];
  m_params.center = center;
  m_params.color = color;
}

Shape::~Shape()
{
  if ( m_vertices != NULL )
    delete [] m_vertices;
}

void Shape::draw()
{
  glBindVertexArray(m_shader.vao);

  // send model-view matrix to shader
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  // for the vertex shader
  glUniform2fv(m_shader.center, 1, m_params.center);
  glUniform2fv(m_shader.translate, 1, m_params.translate);
  glUniform1fv(m_shader.theta, 1, &m_params.theta);
  glUniform2fv(m_shader.scale, 1, m_params.scale);
 
  // for the fragment shader
  glUniform4fv(m_shader.color, 1, m_params.color);
  
  // draw shape
  glDrawArrays(m_shader.drawMode, 0, m_vertex_count);
}

void Shape::init(GLuint program)
{
  GLsizeiptr vertices_size = sizeof(vec4)*m_vertex_count;

  glUseProgram(program);

  glGenVertexArrays(1,&m_shader.vao);
  glBindVertexArray(m_shader.vao);

  glGenBuffers(1,&m_shader.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_shader.vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices_size, m_vertices, GL_STATIC_DRAW);
  
  m_shader.vPosition = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(m_shader.vPosition);
  glVertexAttribPointer(m_shader.vPosition, 4, GL_FLOAT, GL_FALSE, 0,
      BUFFER_OFFSET(0));

  // get locations of uniform variables
  m_shader.center = glGetUniformLocation(program, "center");
  m_shader.translate = glGetUniformLocation(program, "translate");
  m_shader.theta = glGetUniformLocation(program, "theta");
  m_shader.scale = glGetUniformLocation(program, "scale");
  m_shader.color = glGetUniformLocation(program, "color");
}

void Shape::translate(vec2 translate)
{
  m_params.translate.x += translate.x;
  m_params.translate.y += translate.y;
}

void Shape::rotate(GLfloat theta)
{
  m_params.theta += theta;

  // make sure theta stays in [0,360)
  while ( m_params.theta > 360.0 ) m_params.theta -= 360.0;
  while ( m_params.theta <= 0.0  ) m_params.theta += 360.0;
}

void Shape::scale(vec2 scaling)
{
  m_params.scale.x *= scaling.x;
  m_params.scale.y *= scaling.y;
}

bool Shape::isInside(vec2 loc)
{
  return true;
}

void Shape::keyPress(unsigned int key, vec2 loc) {}
void Shape::selectShape(int value) {}

