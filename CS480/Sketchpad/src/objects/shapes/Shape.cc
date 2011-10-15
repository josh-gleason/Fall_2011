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
  // no reason to reset to default values
  resetShape(false);
}

Shape::Shape(const Shape& rhs) :
  m_params(rhs.m_params),
  m_shader(rhs.m_shader),
  m_vertex_count(rhs.m_vertex_count)
{
  if ( m_vertex_count > 0 )
  {
    m_vertices = new vec4[m_vertex_count];
    for ( int i = 0; i < m_vertex_count; ++i )
      m_vertices[i] = rhs.m_vertices[i];

    if ( m_shader.initialized )
    {
      m_shader.initialized = false;
      init(m_shader.program);
    }
  }
}

const Shape& Shape::operator=(const Shape& rhs)
{
  if ( &rhs == this ) return *this;

  // delete any vao/vbo and de-allocate m_vertices
  resetShape(false);

  m_params = rhs.m_params;
  m_shader = rhs.m_shader;
  m_vertices = NULL;
  m_vertex_count = rhs.m_vertex_count;
  
  if ( m_vertex_count > 0 )
  {
    m_vertices = new vec4[m_vertex_count];
    for ( int i = 0; i < m_vertex_count; ++i )
      m_vertices[i] = rhs.m_vertices[i];

    if ( m_shader.initialized )
    {
      m_shader.initialized = false;
      init(m_shader.program);
    }
  }

  return *this;
}

void Shape::reset()
{
  // reset memory as well as return to default values
  resetShape(true);
}

void Shape::resetShape(bool defaultValues)
{
  if ( m_shader.initialized )
  {
    // unbind current vertex array and buffer
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
    // remove buffer and vertex array
    glDeleteBuffers(1,&m_shader.vbo);
    glDeleteVertexArrays(1,&m_shader.vao);
  }

  if ( m_vertices != NULL )
    delete [] m_vertices;
  
  // reset to default values
  if ( defaultValues )
  {
    m_params = ShapeParameters();
    m_shader = ShaderValues();
    m_vertices = NULL;
    m_vertex_count = 0;
  }
}

void Shape::draw() const
{
  ASSERT(m_shader.initialized, "void Shape::init(GLuint program) must" 
         " be called before void Shape::draw()");

  // bind correct vertex array object
  glBindVertexArray(m_shader.vao);
  
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
  ASSERT(!m_shader.initialized, "Shape has already been initialized");
  ASSERT(m_vertices != NULL && m_vertex_count != 0,
         "Shape vertices are undefined");

  m_shader.initialized = true;

  GLsizeiptr vertices_size = sizeof(vec4)*m_vertex_count;

  m_shader.program = program;
  glUseProgram(m_shader.program);

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

void Shape::rotate(GLfloat theta)
{
  m_params.theta += theta;

  // make sure theta stays in [0,360)
  while ( m_params.theta > 360.0 ) m_params.theta -= 360.0;
  while ( m_params.theta <= 0.0  ) m_params.theta += 360.0;
}

void Shape::translate(GLfloat transX, GLfloat transY)
{
  m_params.translate.x += transX;
  m_params.translate.y += transY;
}

void Shape::translate(const vec2& translate)
{
  m_params.translate.x += translate.x;
  m_params.translate.y += translate.y;
}

void Shape::scale(GLfloat scaleX, GLfloat scaleY)
{
  m_params.scale.x *= scaleX;
  m_params.scale.y *= scaleY;
}

void Shape::scale(const vec2& scaling)
{
  m_params.scale.x *= scaling.x;
  m_params.scale.y *= scaling.y;
}

void Shape::setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
  m_params.color = vec4(r,g,b,a);
}

void Shape::setColor(const vec4& color)
{
  m_params.color = color;
}

GLfloat Shape::getTheta() const
{
  return m_params.theta;
}

const vec2& Shape::getTranslation() const
{
  return m_params.translate;
}

const vec2& Shape::getScale() const
{
  return m_params.scale;
}

const vec2& Shape::getCenter() const
{
  return m_params.center+m_params.translate;
}

const vec4& Shape::getColor() const
{
  return m_params.color;
}

// virtual functions meant to be overloaded //////////////////////////

bool Shape::isInside(vec2 loc)
{
  return true;
}

void Shape::keyPress(unsigned int key, vec2 loc) {}
void Shape::selectShape(int value) {}

