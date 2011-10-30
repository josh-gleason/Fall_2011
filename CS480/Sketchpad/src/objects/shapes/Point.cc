#include "Point.h"
#include "SketchPadDefs.h"
#include "PointTests.h"

Point::Point() :
  Shape::Shape()
{
  m_shader.drawMode = GL_POINTS;
  m_params.filled = false;
}

Point::Point(const vec2& loc, const vec4& color, GLfloat thickness) :
  Shape::Shape()
{
  m_params.color = color;
  m_params.thickness = thickness;
  m_params.filled = false;
  m_vertex_count = 1;
  m_vertices = new vec4[m_vertex_count];
  m_shader.drawMode = GL_POINTS;
  m_vertices[0] = vec4(loc.x,loc.y,0.0,1.0);
  m_params.center = loc;
}

Point::Point(const Point& rhs) :
  Shape::Shape(rhs)
{}

bool Point::isInside(vec2 loc) const
{
  if ( !m_shader.initialized ) return false;

  mat4 modelViewInverse = getModelView(true);
  vec4 mouseLoc = modelViewInverse*vec4(loc.x,loc.y,0.0,1.0);
    
  return pointNearPoint( vec2(m_vertices[0].x,m_vertices[0].y),
                         vec2(mouseLoc.x,mouseLoc.y),
                         0.5 * POINT_NEAR_LINE_THRESH );
}
