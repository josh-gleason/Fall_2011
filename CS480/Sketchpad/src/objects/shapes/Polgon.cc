#include "Polygon.h"
#include "SketchPadDefs.h"

Polygon::Polygon() :
  m_vertices(NULL),
  m_vertex_count(0)
{}

Polygon::Polygon(const vec2& vertices, int vertex_count, bool filled,
  const vec4& color, GLfloat thickness)
{
  m_params.color = color;
  m_params.thickness = thickness;

  if ( filled )
    fillShape();
  else
    unFillShape();
}

Polygon::Polygon(const Polygon& rhs) :
  Shape::Shape(rhs)
{}

const Polygon& operator=(const Polygon& rhs)
{
  Shape::operator=(rhs);
}
