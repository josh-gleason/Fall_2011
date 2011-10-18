#include "Poly.h"
#include "SketchPadDefs.h"
#include "InitShader.h"
#include "Tessellate.h"

// TODO: have 2 different lists of vertices so tesselation only needs to be
//       done once per Polygon.

Poly::Poly() : Shape::Shape() {}

Poly::Poly(const vec2* vertices, int vertex_count, bool filled,
  const vec4& color, GLfloat thickness)
{
  m_params.color = color;
  m_params.thickness = thickness;

  // build m_outline which is used in the fill/unFill functions
  for ( int i = 0; i < vertex_count; ++i )
    m_outline.push_back(vertices[i]);

  if ( filled )
    fillShape();
  else
    unFillShape();
}

Poly::Poly(const vec2& startPoint, bool filled, const vec4& color,
  GLfloat thickness)
{
  // TODO
}

Poly::Poly(const Poly& rhs) :
  Shape::Shape(rhs)
{}

const Poly& Poly::operator=(const Poly& rhs)
{
  Shape::operator=(rhs);
}

void Poly::fillShape()
{
  resetShape(false);
  
  m_params.filled = true;
  tessellate(m_outline, &m_vertices, m_vertex_count);
  m_shader.drawMode = GL_TRIANGLES;
  
  // if it was initialized, initialize it again
  if ( m_shader.initialized )
  {
    m_shader.initialized = false;
    init(m_shader.program);
  }
}

void Poly::unFillShape()
{
  resetShape(false);
  
  m_params.filled = false;
  m_vertex_count = m_outline.size();
  m_vertices = new vec4[m_vertex_count];
  for ( int i = 0; i < m_vertex_count; ++i ) 
    m_vertices[i] = vec4(m_outline[i][0], m_outline[i][1], 0.0, 1.0);
  m_shader.drawMode = GL_LINE_LOOP;
  
  // if it was initialized, initialize it again
  if ( m_shader.initialized )
  {
    m_shader.initialized = false;
    init(m_shader.program);
  }
}

bool Poly::isInside(vec2 loc) const
{
  return true;// TODO
}

void Poly::mouseDown(vec2 cameraCoordLoc, int mode)
{
  // TODO  
}

void Poly::mouseMove(vec2 cameraCorrdLoc, int mode)
{
  // TODO  
}

void Poly::mouseUp(vec2 cameraCorrdLoc, int mode)
{
  // TODO  
}

