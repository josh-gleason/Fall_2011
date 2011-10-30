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
  m_shader.drawMode = GL_LINE_STRIP;

  m_vertex_count = vertex_count;
  m_vertices = new vec4[m_vertex_count];

  // build actual vertices for use with GL_LINES
  for ( int i = 0; i < m_vertex_count; ++i )
    m_vertices[i] = vec4(vertices[i].x,vertices[i].y,0.0,1.0);

  mat2 bbox = getBoundingBox();
  m_params.center = vec2( (bbox[0][0]+bbox[1][0])/2.0,
                          (bbox[0][1]+bbox[1][1])/2.0 );
}

LineSegs::LineSegs(const vec2& startPoint, const vec4& color,
  GLfloat thickness) :
  Shape::Shape()
{
  m_params.color = color;
  m_params.thickness = thickness;
  m_shader.drawMode = GL_LINE_STRIP;
  
  m_vertex_count = 1;
  m_vertices = new vec4[1];
  m_vertices[0] = vec4(startPoint.x,startPoint.y,0.0,1.0);

  m_params.center = startPoint;
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

  GLfloat scaledThresh =
    (1.0/fabs(m_params.scale.x) + 1.0/fabs(m_params.scale.y)) * 0.5 * POINT_NEAR_LINE_THRESH;

  if ( !( mouseTemp.x >= bbox[0][0]-scaledThresh && mouseTemp.x <= bbox[1][0]+scaledThresh 
       && mouseTemp.y >= bbox[0][1]-scaledThresh && mouseTemp.y <= bbox[1][1]+scaledThresh ) )
    return false;
  
  vec2 mouseLoc = vec2(mouseTemp.x, mouseTemp.y);

  // test all line segments
  for ( unsigned i = 0; i < m_vertex_count-1; i++ )
    if ( pointNearLineSegment(&(m_vertices[i]), mouseLoc, scaledThresh) )
      return true;
  
  return false;
}
    
void LineSegs::mouseDownChild(vec2 cameraCoordLoc, int mode)
{
  switch (mode)
  {
    case MODE_DRAW_LINE_SEG:
    case MODE_DRAW_POLY:
      int new_vertex_count = m_vertex_count+1;
      vec4* new_vertices = new vec4[new_vertex_count];
      
      for ( int i = 0; i < m_vertex_count; ++i )
        new_vertices[i] = m_vertices[i];
      
      new_vertices[m_vertex_count] =
        vec4(cameraCoordLoc.x,cameraCoordLoc.y,0.0,1.0);
      
      resetShape(false);
      delete [] m_vertices;

      m_vertices = new_vertices;
      m_vertex_count = new_vertex_count;
      
      if ( m_shader.initialized )
      {
        m_shader.initialized = false;
        init(m_shader.program);
      }
      
      mat2 bbox = getBoundingBox();
      m_params.center = vec2( (bbox[0][0]+bbox[1][0])/2.0,
                              (bbox[0][1]+bbox[1][1])/2.0 );
      break;
  }
}

void LineSegs::mouseMoveChild(vec2 cameraCoordLoc, int mode)
{
  switch (mode)
  {
    case MODE_DRAW_LINE_SEG:
    case MODE_DRAW_POLY:
      m_vertices[m_vertex_count-1] = 
        vec4(cameraCoordLoc.x,cameraCoordLoc.y,0.0,1.0);

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
      
      mat2 bbox = getBoundingBox();
      m_params.center = vec2( (bbox[0][0]+bbox[1][0])/2.0,
                              (bbox[0][1]+bbox[1][1])/2.0 );
      break;
  }
}

void LineSegs::mouseUpChild(vec2 cameraCoordLoc, int mode)
{
  switch (mode)
  {
    case MODE_DRAW_LINE_SEG:
    case MODE_DRAW_POLY:
      // nothing to do
      break;
  }
}
