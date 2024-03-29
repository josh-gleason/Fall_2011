#include "Poly.h"
#include "SketchPadDefs.h"
#include "InitShader.h"
#include "Tessellate.h"
#include "PointTests.h"

// TODO: have 2 different lists of vertices so tesselation only needs to be
//       done once per Polygon.

Poly::Poly() : 
  Shape::Shape(),
  m_vertices_filled(NULL),
  m_vertices_outline(NULL),
  m_vertex_count_filled(0),
  m_vertex_count_outline(0)
{}

Poly::Poly(const vec2* vertices, int vertex_count, bool filled,
  const vec4& color, GLfloat thickness) :
  Shape::Shape()
{
  m_params.color = color;
  m_params.thickness = thickness;

  // build m_vertices_outline
  m_vertex_count_outline = vertex_count;
  m_vertices_outline = new vec4[m_vertex_count_outline];

  for ( unsigned i = 0; i < m_vertex_count_outline; ++i )
    m_vertices_outline[i] = vec4(vertices[i].x, vertices[i].y, 0.0, 1.0);

  // builds m_vertices_filled and sets m_vertex_count_filled
  tessellate(m_vertices_outline, m_vertex_count_outline,
             &m_vertices_filled, m_vertex_count_filled);

  // sets m_vertices to either m_vertices_outline or m_vertices_filled
  if ( filled )
    fillShape();
  else
    unFillShape();
      
  mat2 bbox = getBoundingBox();
  m_params.center = vec2( (bbox[0][0]+bbox[0][1])/2.0,
                          (bbox[0][0]+bbox[0][1])/2.0 );
}

Poly::Poly(const LineSegs& rhs)
{
  m_params = rhs.m_params;
  m_shader = rhs.m_shader;

  // build m_vertices_outline
  m_vertex_count_outline = rhs.m_vertex_count;
  m_vertices_outline = new vec4[m_vertex_count_outline];

  for ( unsigned i = 0; i < m_vertex_count_outline; ++i )
    m_vertices_outline[i] =
      vec4(rhs.m_vertices[i].x, rhs.m_vertices[i].y, 0.0, 1.0);

  // builds m_vertices_filled and sets m_vertex_count_filled
  tessellate(m_vertices_outline, m_vertex_count_outline,
             &m_vertices_filled, m_vertex_count_filled);

  // sets m_vertices to either m_vertices_outline or m_vertices_filled
  if ( m_params.filled )
    fillShape();
  else
    unFillShape();
  
  if ( m_shader.initialized )
  {
    m_shader.initialized = false;
    init(m_shader.program);
  }
}

Poly::Poly(const Poly& rhs)
{
  m_params = rhs.m_params;
  m_shader = rhs.m_shader;
  m_vertex_count = rhs.m_vertex_count;
  m_vertex_count_filled = rhs.m_vertex_count_filled;
  m_vertex_count_outline = rhs.m_vertex_count_outline;

  if ( &rhs == this ) return;
  
  m_vertices_filled = NULL;
  m_vertices_outline = NULL;
  
  if ( rhs.m_vertices_filled != NULL )
  {
    m_vertices_filled = new vec4[m_vertex_count_filled];

    for ( unsigned i = 0; i < m_vertex_count_filled; ++i )
      m_vertices_filled[i] = rhs.m_vertices_filled[i];

    if ( rhs.m_vertices_filled == rhs.m_vertices )
      m_vertices = m_vertices_filled;
  }
 
  if ( rhs.m_vertices_outline != NULL )
  {
    m_vertices_outline = new vec4[m_vertex_count_outline];

    for ( unsigned i = 0; i < m_vertex_count_outline; ++i )
      m_vertices_outline[i] = rhs.m_vertices_outline[i];
    
    if ( rhs.m_vertices_outline == rhs.m_vertices )
      m_vertices = m_vertices_outline;
  }

  if ( m_vertices != NULL && m_shader.initialized )
  {
    m_shader.initialized = false;
    init(m_shader.program);
  }
}

Poly::~Poly()
{
  m_vertices = NULL;  // because it is pointing to another list
  m_vertex_count = 0;
  resetShape(false);
  if ( m_vertices_filled != NULL )
    delete [] m_vertices_filled;
  if ( m_vertices_outline != NULL )
    delete [] m_vertices_outline;
}

const Poly& Poly::operator=(const Poly& rhs)
{
  if ( &rhs == this ) return *this;

  // delete any vao/vbo, m_vertices is a pointer to something else
  // so we don't want to de-allocate it
  m_vertices = NULL; 
  resetShape(false);
  
  if ( m_vertices_filled != NULL )
  {
    delete [] m_vertices_filled;
    m_vertices_filled = NULL;
  }
  if ( m_vertices_outline != NULL )
  {
    delete [] m_vertices_outline;
    m_vertices_outline = NULL;
  }

  m_params = rhs.m_params;
  m_shader = rhs.m_shader;  // gets copied but vao and vbo get re-initialized
  m_vertex_count = rhs.m_vertex_count;
 
  if ( rhs.m_vertices_filled != NULL )
  {
    m_vertex_count_filled = rhs.m_vertex_count_filled;
    m_vertices_filled = new vec4[m_vertex_count_filled];

    for ( unsigned i = 0; i < m_vertex_count_filled; ++i )
      m_vertices_filled[i] = rhs.m_vertices_filled[i];

    if ( rhs.m_vertices_filled == rhs.m_vertices )
      m_vertices = m_vertices_filled;
  }
 
  if ( rhs.m_vertices_outline != NULL )
  {
    m_vertex_count_outline = rhs.m_vertex_count_outline;
    m_vertices_outline = new vec4[m_vertex_count_outline];

    for ( unsigned i = 0; i < m_vertex_count_outline; ++i )
      m_vertices_outline[i] = rhs.m_vertices_outline[i];
    
    if ( rhs.m_vertices_outline == rhs.m_vertices )
      m_vertices = m_vertices_outline;
  }

  if ( m_vertices != NULL && m_shader.initialized )
  {
    m_shader.initialized = false;
    init(m_shader.program);
  }

  return *this;
}

void Poly::fillShape()
{
  ASSERT(m_vertices_outline != NULL,
    "Attempting to draw filled polygon but vertices have not been defined");
  
  m_vertices = NULL;
  resetShape(false);
  
  m_params.filled = true;
  m_shader.drawMode = GL_TRIANGLES;
  m_vertices = m_vertices_filled;
  m_vertex_count = m_vertex_count_filled;
  
  // if it was initialized, initialize it again
  if ( m_shader.initialized )
  {
    m_shader.initialized = false;
    init(m_shader.program);
  }
}

void Poly::unFillShape()
{
  ASSERT(m_vertices_outline != NULL,
    "Attempting to draw outline but vertices have not been defined");

  m_vertices = NULL;
  resetShape(false);
  
  m_params.filled = false;
  m_shader.drawMode = GL_LINE_LOOP;
  m_vertices = m_vertices_outline;
  m_vertex_count = m_vertex_count_outline;
  
  // if it was initialized, initialize it again
  if ( m_shader.initialized )
  {
    m_shader.initialized = false;
    init(m_shader.program);
  }
}

bool Poly::isInside(vec2 loc) const
{
  if ( !m_shader.initialized ) return false;
  if ( m_vertices_filled==NULL ) return false;
  
  // bounding box check first
  mat2 bbox = getBoundingBox();
  mat4 modelViewInverse = getModelView(true);

  // get mouse location in model coordinates
  vec4 mouseLoc = modelViewInverse*vec4(loc.x,loc.y,0,1);
  vec2 mouseLoc2 = vec2(mouseLoc.x, mouseLoc.y);
    
  GLfloat scaledThresh =
    (1.0/fabs(m_params.scale.x) + 1.0/fabs(m_params.scale.y)) * 0.5 * POINT_NEAR_LINE_THRESH;

  if ( !( mouseLoc.x >= bbox[0][0]-scaledThresh && mouseLoc.x <= bbox[1][0]+scaledThresh 
       && mouseLoc.y >= bbox[0][1]-scaledThresh && mouseLoc.y <= bbox[1][1]+scaledThresh ) )
    return false;

  if ( m_params.filled )  // triangles
  {
    for ( unsigned i = 0; i < m_vertex_count_filled; i+=3 )
      if ( pointInTriangle(&(m_vertices_filled[i]), mouseLoc2) )
        return true;
  }
  else    // lines
  {
    for ( unsigned i = 0; i < m_vertex_count_outline-1; i++ )
      if ( pointNearLineSegment(&(m_vertices_outline[i]), mouseLoc2,
                                  scaledThresh) )
        return true;
    
    vec4 lastFirst[2] = {m_vertices_outline[m_vertex_count_outline-1],
                         m_vertices_outline[0]};
    if ( pointNearLineSegment(lastFirst, mouseLoc2, scaledThresh) )
      return true;
  }

  return false;
}

void Poly::mouseDownChild(vec2 cameraCoordLoc, int mode)
{}

void Poly::mouseMoveChild(vec2 cameraCorrdLoc, int mode)
{}

void Poly::mouseUpChild(vec2 cameraCorrdLoc, int mode)
{}

void Poly::Save(std::ofstream &fout) const
{
  // don't save uninitialized shapes
  if ( !m_shader.initialized )
    return;

  fout << "P" << std::endl;
  fout << m_params.center.x << ' ' << m_params.center.y << std::endl;
  fout << m_params.translate.x << ' ' << m_params.translate.y << std::endl;
  fout << m_params.theta << std::endl;
  fout << m_params.scale.x << ' ' << m_params.scale.y << std::endl;
  fout << m_params.color.x << ' ' << m_params.color.y << ' ' 
       << m_params.color.z << ' ' << m_params.color.w << std::endl;
  fout << m_params.thickness << std::endl;
  fout << m_params.filled << std::endl;

  fout << m_vertex_count_outline << std::endl;
  for ( int i = 0; i < m_vertex_count_outline; ++i )
    fout << m_vertices_outline[i].x << ' ' << m_vertices_outline[i].y << ' '
         << m_vertices_outline[i].z << ' ' << m_vertices_outline[i].w << ' ';
  fout << std::endl;
}

void Poly::Load(std::ifstream &fin, GLuint program)
{
  m_vertices = NULL;  // because it is pointing to another list
  m_vertex_count = 0;
  resetShape(true);
  if ( m_vertices_filled != NULL )
    delete [] m_vertices_filled;
  if ( m_vertices_outline != NULL )
    delete [] m_vertices_outline;
  m_vertices_filled = NULL;
  m_vertices_outline = NULL;
  m_vertex_count_filled = 0;
  m_vertex_count_outline = 0;

  fin >> m_params.center.x >> m_params.center.y
      >> m_params.translate.x >> m_params.translate.y
      >> m_params.theta
      >> m_params.scale.x >> m_params.scale.y
      >> m_params.color.x >> m_params.color.y
      >> m_params.color.z >> m_params.color.w
      >> m_params.thickness
      >> m_params.filled;
 
  fin >> m_vertex_count_outline;
  m_vertices_outline = new vec4[m_vertex_count_outline];
  for ( int i = 0; i < m_vertex_count_outline; ++i )
    fin >> m_vertices_outline[i].x >> m_vertices_outline[i].y
        >> m_vertices_outline[i].z >> m_vertices_outline[i].w;
  
  // builds m_vertices_filled and sets m_vertex_count_filled
  tessellate(m_vertices_outline, m_vertex_count_outline,
             &m_vertices_filled, m_vertex_count_filled);

  if ( m_params.filled ) fillShape();
  else                   unFillShape();

  init(program);
}
