#include <GL/glew.h>
#include <GL/glut.h>
#include "Shape.h"
#include "InitShader.h"

Shape::Shape() :
  m_vertices(NULL),
  m_vertex_count(0)
{}

Shape::Shape(int numVertices, GLenum drawMode, const vec4* vertices,
  const vec4& color, const vec2& center) :
  m_vertices(new vec4[numVertices]),
  m_vertex_count(numVertices)
{
  m_vertices = new vec4[numVertices];
  m_vertex_count = numVertices;
  if ( vertices != NULL )
    for ( int i = 0; i < numVertices; ++i )
      m_vertices[i] = vertices[i];
  m_params.center = center;
  m_params.color = color;
  m_shader.drawMode = drawMode;
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
  // don't draw something that is scaled too small
  if ( fabs(m_params.scale.x) < 0.000011 && fabs(m_params.scale.y) < 0.000011 )
    return;

  ASSERT(m_shader.initialized, "void Shape::init(GLuint program) must" 
         " be called before void Shape::draw()");

  // bind correct vertex array object
  glBindVertexArray(m_shader.vao);
 
  if ( !m_params.filled )
    glLineWidth(m_params.thickness);

  // for the vertex shader
  glUniform2fv(m_shader.center, 1, m_params.center);
  glUniform2fv(m_shader.translate, 1, m_params.translate);
  glUniform1fv(m_shader.theta, 1, &m_params.theta);
  glUniform2fv(m_shader.scale, 1, m_params.scale);
 
  // for the fragment shader
  glUniform4fv(m_shader.color, 1, m_params.color);
  
  // draw shape
  glDrawArrays(m_shader.drawMode, 0, m_vertex_count);
  
  if ( !m_params.filled )
    glLineWidth(1.0);
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
  setScale(m_params.scale*scaling);
}

// protected
void Shape::setScale(const vec2& scaling)
{
  m_params.scale = scaling;
/*
  if ( fabs(m_params.scale.x) < 0.00001 )
    m_params.scale.x = 0.00001;
  if ( fabs(m_params.scale.y) < 0.00001 )
    m_params.scale.y = 0.00001;*/
}

void Shape::setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
  m_params.color = vec4(r,g,b,a);
}

void Shape::setColor(const vec4& color)
{
  m_params.color = color;
}

void Shape::setThickness(GLfloat thickness)
{
  m_params.thickness = thickness;
}

void Shape::setFilled(bool filled)
{
  if ( filled == m_params.filled ) return;
 
  if ( filled )
    fillShape();
  else
    unFillShape();
}

void Shape::toggleFilled()
{
  setFilled(!m_params.filled);
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

const vec2 Shape::getCenter() const
{
  return m_params.center+m_params.translate;
}

const vec4& Shape::getColor() const
{
  return m_params.color;
}

const ShapeParameters& Shape::getParams() const
{
  return m_params;
}

GLenum Shape::getDrawMode() const
{
  return m_shader.drawMode;
}
    
GLfloat Shape::getThickness() const
{
  return m_params.thickness;
}

bool Shape::getFilled() const
{
  return m_params.filled;
}
    
mat2 Shape::getBoundingBox() const
{
  ASSERT(m_shader.initialized && m_vertex_count > 0,"");

  // determine bounding box
  vec2 minVal(m_vertices[0].x, m_vertices[0].y);
  vec2 maxVal(minVal);
  
  for ( int i = 0; i < m_vertex_count; ++i )
  {
    if ( m_vertices[i].x < minVal.x )
      minVal.x = m_vertices[i].x;
    if ( m_vertices[i].y < minVal.y )
      minVal.y = m_vertices[i].y;
    if ( m_vertices[i].x > maxVal.x )
      maxVal.x = m_vertices[i].x;
    if ( m_vertices[i].y > maxVal.y )
      maxVal.y = m_vertices[i].y;
  }

  return mat2(minVal,
              maxVal);
}

mat4 Shape::getModelView(bool inverse) const
{
  GLfloat thetaRads = m_params.theta * M_PI / 180.0;
  GLfloat c = cos(thetaRads);
  GLfloat s = sin(thetaRads);

  // compute model-view matrix, based computing the follow transforms in order
  // trans(-center)->rotate(theta)->scale(scale)->trans(center+translate)
  GLfloat m11 = c*m_params.scale.x;
  GLfloat m12 = -s*m_params.scale.x;
  GLfloat m21 = s*m_params.scale.y;
  GLfloat m22 = c*m_params.scale.y;

  GLfloat m14 = m_params.center.x*(1.0-c*m_params.scale.x)+m_params.center.y
      * m_params.scale.x*s+m_params.translate.x;

  GLfloat m24 = m_params.center.y*(1.0-c*m_params.scale.y)-m_params.center.x
      * m_params.scale.y*s+m_params.translate.y;
                      
  if ( !inverse )
  {
    return mat4(m11, m12, 0.0, m14,
                m21, m22, 0.0, m24,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0);
  }
  else
  {
    GLfloat det = m11*m22-m12*m21;
    
    // non-transposed inverse
    return mat4(m22/det, -m12/det, 0.0,  (m12*m24-m14*m22)/det,
               -m21/det,  m11/det, 0.0, -(m11*m24-m14*m21)/det,
                0.0,      0.0,     1.0,  0.0,
                0.0,      0.0,     0.0,  1.0);
    
  }
}

// virtual functions meant to be overloaded //////////////////////////

bool Shape::isInside(vec2 loc) const
{
  // simple bounding box test

  mat2 bbox = getBoundingBox();
  mat4 modelViewInverse = getModelView(true);

  // get mouse location in model coordinates
  vec4 mouseLoc = modelViewInverse*vec4(loc.x,loc.y,0,1);

  return ( mouseLoc.x >= bbox[0][0] && mouseLoc.x <= bbox[1][0] 
        && mouseLoc.y >= bbox[0][1] && mouseLoc.y <= bbox[1][1] );
}

void Shape::mouseDown(vec2 cameraCoordLoc, int mode) {}
void Shape::mouseMove(vec2 cameraCoordLoc, int mode) {}
void Shape::mouseUp(vec2 cameraCoordLoc, int mode) {}

void Shape::toggleSelectShape(int value)
{
  m_params.selected = !m_params.selected;
}

void Shape::selectShape(int value)
{
  m_params.selected = true;
}

void Shape::unSelectShape(int value)
{
  m_params.selected = false;
}

void Shape::fillShape()
{
  m_params.filled = true;
}

void Shape::unFillShape()
{
  m_params.filled = false;
}

