#include <GL/glew.h>
#include <GL/glut.h>

#include "Shape.h"
#include "InitShader.h"

Shape::Shape() :
  m_vertices(NULL),
  m_vertex_count(0)
{}

Shape::Shape(int numVertecies, const vec4* vertices) :
  m_vertices(new vec4[numVertecies]),
  m_vertex_count(numVertecies)
{
  if ( vertices != NULL )
    for ( int i = 0; i < numVertecies; ++i )
      m_vertices[i] = vertices[i];
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
  /*glTranslatef(-m_params.center.x,
               -m_params.center.y,
               -m_params.center.z);
  */
  glRotatef(m_params.rotate.z, 0, 0, 1);
  glScalef(m_params.scale.x,
           m_params.scale.y,
           m_params.scale.z);
  /*glTranslatef(m_params.center.x+m_params.translate.x,
               m_params.center.y+m_params.translate.y,
               m_params.center.z+m_params.translate.z);*/
  glUniform4fv(m_shader.uColor, 1, m_params.color);
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

  m_shader.uColor = glGetUniformLocation(program, "uColor");
}

void Shape::translate(vec2 translation)
{
  m_params.translate.x += translation.x;
  m_params.translate.y += translation.y;
}

void Shape::rotate(GLfloat theta)
{
  m_params.rotate.z += theta;

  // make sure theta stays in [0,360)
  while ( m_params.rotate.z > 360.0 ) m_params.rotate.z -= 360.0;
  while ( m_params.rotate.z <= 0.0  ) m_params.rotate.z += 360.0;
}

void Shape::scale(vec2 scaling)
{
  m_params.scale.x *= scaling.x;
  m_params.scale.y *= scaling.y;
}

void keyPress(unsigned int key, vec2 loc) {}
void selectShape(int value) {}

