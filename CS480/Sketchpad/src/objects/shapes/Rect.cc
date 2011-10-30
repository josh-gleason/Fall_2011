#include "Rect.h"
#include "SketchPadDefs.h"

Rect::Rect() : Shape::Shape() {}

Rect::Rect(const vec2& upperLeft, const vec2& lowerRight, bool filled,
  const vec4& color, GLfloat thickness) :
  Shape::Shape(),
  m_upperLeft(upperLeft),
  m_lowerRight(lowerRight)
{
  m_params.center = (upperLeft+lowerRight)*0.5;
  m_params.color = color;
  m_params.thickness = thickness;

  m_params.filled = filled;

  if ( filled ) fillShape();
  else          unFillShape();
}
    
Rect::Rect(const vec2& startPoint, bool filled, const vec4& color,
  GLfloat thickness) :
  Shape::Shape(),
  m_upperLeft(startPoint),
  m_lowerRight(startPoint)
{
  m_params.center = m_upperLeft;

  m_params.color = color;
  m_params.thickness = thickness;

  if ( filled ) fillShape();
  else          unFillShape();
}

Rect::Rect(const Rect& rhs) :
  Shape::Shape(rhs),
  m_upperLeft(rhs.m_upperLeft),
  m_lowerRight(rhs.m_lowerRight)
{}

const Rect& Rect::operator=(const Rect& rhs)
{
  Shape::operator=(rhs);
  return *this;
}

vec2 Rect::getSize() const
{
  if ( m_vertices == NULL ) return vec2(0);

  // return width and height
  return vec2( m_params.scale.x * fabs(m_vertices[1].x - m_vertices[0].x),
               m_params.scale.y * fabs(m_vertices[3].y - m_vertices[0].x) );
}

void Rect::fillShape()
{
  if ( m_vertices == NULL || m_vertex_count != 4 )
  {
    if ( m_vertices != NULL )
      delete [] m_vertices;
    m_vertex_count = 4;
    m_vertices = new vec4[m_vertex_count];
  }

  m_vertices[0] = vec4(m_upperLeft.x, m_upperLeft.y, 0.0, 1.0);
  m_vertices[1] = vec4(m_lowerRight.x, m_upperLeft.y, 0.0, 1.0);
  m_vertices[2] = vec4(m_upperLeft.x, m_lowerRight.y, 0.0, 1.0);
  m_vertices[3] = vec4(m_lowerRight.x, m_lowerRight.y, 0.0, 1.0);

  m_params.filled = true;

  m_shader.drawMode = GL_TRIANGLE_STRIP;

  // if it was initialized, initialize it again
  if ( m_shader.initialized )
  {
    // unbind current vertex array and buffer
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
    // remove buffer and vertex array
    glDeleteBuffers(1,&m_shader.vbo);
    glDeleteVertexArrays(1,&m_shader.vao);
    
    m_shader.initialized = false;
    init(m_shader.program);
  }
}

void Rect::unFillShape()
{
  // delete a shader if it exists as well as the vertices
  if ( m_vertices == NULL || m_vertex_count != 4 )
  {
    if ( m_vertices != NULL )
      delete [] m_vertices;
    m_vertex_count = 4;
    m_vertices = new vec4[m_vertex_count];
  }

  m_vertices[0] = vec4(m_upperLeft.x, m_upperLeft.y, 0.0, 1.0);
  m_vertices[1] = vec4(m_lowerRight.x, m_upperLeft.y, 0.0, 1.0);
  m_vertices[2] = vec4(m_lowerRight.x, m_lowerRight.y, 0.0, 1.0);
  m_vertices[3] = vec4(m_upperLeft.x, m_lowerRight.y, 0.0, 1.0);

  m_params.filled = false;

  m_shader.drawMode = GL_LINE_LOOP;

  // if it was initialized, initialize it again
  if ( m_shader.initialized )
  {
    // unbind current vertex array and buffer
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
    // remove buffer and vertex array
    glDeleteBuffers(1,&m_shader.vbo);
    glDeleteVertexArrays(1,&m_shader.vao);
    
    m_shader.initialized = false;
    init(m_shader.program);
  }
}
  
void Rect::mouseDownChild(vec2 cameraCoordLoc, int mode)
{
  switch (mode)
  {
    case MODE_DRAW_RECT: break;    // nothing to do
  }
}

void Rect::mouseMoveChild(vec2 cameraCoordLoc, int mode)
{
  switch (mode)
  {
    case MODE_DRAW_RECT:
      m_lowerRight = cameraCoordLoc;
      m_params.center = 0.5*(m_upperLeft + m_lowerRight);
      if ( m_params.filled )
        fillShape();
      else
        unFillShape();
      break;
  }
}

void Rect::mouseUpChild(vec2 cameraCoordLoc, int mode)
{
  switch (mode)
  {
    case MODE_DRAW_RECT:
      break;
  }
}

void Rect::Save(std::ofstream &fout) const
{
  // don't save uninitialized shapes
  if ( !m_shader.initialized )
    return;

  fout << "R" << std::endl;
  fout << m_params.center.x << ' ' << m_params.center.y << std::endl;
  fout << m_params.translate.x << ' ' << m_params.translate.y << std::endl;
  fout << m_params.theta << std::endl;
  fout << m_params.scale.x << ' ' << m_params.scale.y << std::endl;
  fout << m_params.color.x << ' ' << m_params.color.y << ' ' 
       << m_params.color.z << ' ' << m_params.color.w << std::endl;
  fout << m_params.thickness << std::endl;
  fout << m_params.filled << std::endl;

  fout << m_upperLeft.x << ' ' << m_upperLeft.y << std::endl;
  fout << m_lowerRight.x << ' ' << m_lowerRight.y << std::endl;
}

void Rect::Load(std::ifstream &fin, GLuint program)
{
  resetShape(true);

  fin >> m_params.center.x >> m_params.center.y
      >> m_params.translate.x >> m_params.translate.y
      >> m_params.theta
      >> m_params.scale.x >> m_params.scale.y
      >> m_params.color.x >> m_params.color.y
      >> m_params.color.z >> m_params.color.w
      >> m_params.thickness
      >> m_params.filled;

  fin >> m_upperLeft.x >> m_upperLeft.y
      >> m_lowerRight.x >> m_lowerRight.y;

  if ( m_params.filled ) fillShape();
  else                   unFillShape();

  init(program);
}

