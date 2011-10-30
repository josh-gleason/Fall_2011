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

void Point::Save(std::ofstream &fout) const
{
  // don't save uninitialized shapes
  if ( !m_shader.initialized )
    return;

  fout << "T" << std::endl;
  fout << m_params.center.x << ' ' << m_params.center.y << std::endl;
  fout << m_params.translate.x << ' ' << m_params.translate.y << std::endl;
  fout << m_params.theta << std::endl;
  fout << m_params.scale.x << ' ' << m_params.scale.y << std::endl;
  fout << m_params.color.x << ' ' << m_params.color.y << ' ' 
       << m_params.color.z << ' ' << m_params.color.w << std::endl;
  fout << m_params.thickness << std::endl;
  fout << m_params.filled << std::endl;

  fout << m_vertex_count << std::endl;
  for ( int i = 0; i < m_vertex_count; ++i )
    fout << m_vertices[i].x << ' ' << m_vertices[i].y << ' '
         << m_vertices[i].y << ' ' << m_vertices[i].w << ' ';
  fout << std::endl;
}

void Point::Load(std::ifstream &fin, GLuint program)
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
 
  fin >> m_vertex_count;
  m_vertices = new vec4[m_vertex_count];
  for ( int i = 0; i < m_vertex_count; ++i )
    fin >> m_vertices[i].x >> m_vertices[i].y
        >> m_vertices[i].z >> m_vertices[i].w;

  m_shader.drawMode = GL_POINTS;

  init(program);
}
