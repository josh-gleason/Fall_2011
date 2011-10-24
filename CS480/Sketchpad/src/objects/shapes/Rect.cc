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
  m_lowerRight(startPoint+vec2(1.0,-1.0))
{
  // so that scaling takes place from start point
  m_params.center = m_upperLeft;

  m_params.color = color;
  m_params.thickness = thickness;

  if ( filled ) fillShape();
  else          unFillShape();

  // make the object as small as possible
  scale(0.0,0.0);
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
  // delete a shader if it exists as well as the vertices
  resetShape(false);
  
  m_vertex_count = 4;
  m_vertices = new vec4[m_vertex_count];

  m_vertices[0] = vec4(m_upperLeft.x, m_upperLeft.y, 0.0, 1.0);
  m_vertices[1] = vec4(m_lowerRight.x, m_upperLeft.y, 0.0, 1.0);
  m_vertices[2] = vec4(m_upperLeft.x, m_lowerRight.y, 0.0, 1.0);
  m_vertices[3] = vec4(m_lowerRight.x, m_lowerRight.y, 0.0, 1.0);

  m_params.filled = true;

  m_shader.drawMode = GL_TRIANGLE_STRIP;

  // if it was initialized, initialize it again
  if ( m_shader.initialized )
  {
    m_shader.initialized = false;
    init(m_shader.program);
  }
}

void Rect::unFillShape()
{
  // delete a shader if it exists as well as the vertices
  resetShape(false);
  
  m_vertex_count = 4;
  m_vertices = new vec4[m_vertex_count];

  m_vertices[0] = vec4(m_upperLeft.x, m_upperLeft.y, 0.0, 1.0);
  m_vertices[1] = vec4(m_lowerRight.x, m_upperLeft.y, 0.0, 1.0);
  m_vertices[2] = vec4(m_lowerRight.x, m_lowerRight.y, 0.0, 1.0);
  m_vertices[3] = vec4(m_upperLeft.x, m_lowerRight.y, 0.0, 1.0);

  m_params.filled = false;

  m_shader.drawMode = GL_LINE_LOOP;

  // if it was initialized, initialize it again
  if ( m_shader.initialized )
  {
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
      setScale(vec2(cameraCoordLoc.x - m_upperLeft.x,
                    m_upperLeft.y - cameraCoordLoc.y));
      break;
  }
}

void Rect::mouseUpChild(vec2 cameraCoordLoc, int mode)
{
  switch (mode)
  {
    case MODE_DRAW_RECT:
      // need to set the center to the actual center now
      setScale(vec2(cameraCoordLoc.x - m_upperLeft.x,
                    m_upperLeft.y - cameraCoordLoc.y));
      m_params.center = (m_upperLeft+m_lowerRight)*0.5;
      translate(vec2(m_params.center.x-m_upperLeft.x-1+m_params.scale.x*0.5,
                     m_upperLeft.y-m_params.center.y-m_params.scale.y*0.5));
      break;
  }
}

