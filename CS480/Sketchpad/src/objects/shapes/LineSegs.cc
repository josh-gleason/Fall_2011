#include "LineSegs.h"
#include "PointTests.h"
#include "SketchPadDefs.h"

LineSegs::LineSegs() :
  Shape::Shape()
{}

LineSegs::LineSegs(const vec2* vertices, int vertex_count, const vec4& color,
  GLfloat thickness) :
  Shape::Shape()
{
  m_params.color = color;
  m_params.thickness = thickness;
  m_vertex_count = vertex_count;
  m_shader.drawMode = GL_LINE_STRIP;

  m_vertices = new vec4[m_vertex_count];

  // build actual vertices for use with GL_LINES
  for ( int i = 0; i < vertex_count; ++i )
    m_vertices[i] = vec4(vertices[i].x,vertices[i].y,0.0,1.0);
}

LineSegs::LineSegs(const vec2& startPoint, const vec4& color,
  GLfloat thickness)
{
  // TODO
}

LineSegs::LineSegs(const LineSegs& rhs) :
  Shape::Shape(rhs)
{}

const LineSegs& LineSegs::operator=(const LineSegs& rhs)
{
  Shape::operator=(rhs);
  return *this;
}

bool LineSegs::isInside(vec2 loc) const
{
  if ( !m_shader.initialized ) return false;
  if ( m_vertices==NULL ) return false;
  
  // bounding box check first
  mat2 bbox = getBoundingBox();
  mat4 modelViewInverse = getModelView(true);

  // get mouse location in model coordinates
  vec4 mouseTemp = modelViewInverse*vec4(loc.x,loc.y,0,1);

  if ( !( mouseTemp.x >= bbox[0][0] && mouseTemp.x <= bbox[1][0] 
       && mouseTemp.y >= bbox[0][1] && mouseTemp.y <= bbox[1][1] ) )
    return false;
  
  vec2 mouseLoc = vec2(mouseTemp.x, mouseTemp.y);

  GLfloat scaledThresh =
    (1.0/m_params.scale.x + 1.0/m_params.scale.y) * 0.5 * POINT_NEAR_LINE_THRESH;
 
  // test all line segments
  for ( unsigned i = 0; i < m_vertex_count-1; i++ )
    if ( pointNearLineSegment(&(m_vertices[i]), mouseLoc, scaledThresh) )
      return true;
  
  return false;
}
    
void LineSegs::mouseDownChild(vec2 cameraCoordLoc, int mode)
{
  // TODO
}

void LineSegs::mouseMoveChild(vec2 cameraCoordLoc, int mode)
{
  // TODO
}

void LineSegs::mouseUpChild(vec2 cameraCoordLoc, int mode)
{
  // TODO
}
