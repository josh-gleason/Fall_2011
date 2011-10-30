#include "Circle.h"
#include "SketchPadDefs.h"

Circle::Circle() : Shape::Shape() {}

Circle::Circle(const vec2& center, GLfloat radius, bool filled, const vec4& color,
  GLfloat thickness) :
  Shape::Shape(),
  m_radius(radius)
{
  m_params.filled = filled;
  m_params.color = color;
  m_params.center = center;
  m_params.thickness = thickness;

  m_vertex_count = 32;
  m_vertices = new vec4[m_vertex_count];
  GLfloat theta = 0.0;
  GLfloat stepSize = 2.0*M_PI/m_vertex_count;
  for ( int i = 0; i < m_vertex_count; ++i )
  {
    m_vertices[i] = vec4(m_radius*cos(theta)+m_params.center.x,
                         m_radius*sin(theta)+m_params.center.y,
                         0.0,1.0);
    theta+=stepSize;
  }
  
  if ( m_params.filled ) fillShape();
  else                   unFillShape();

}

Circle::Circle(const vec2& startPoint, bool filled, const vec4& color,
  GLfloat thickness) :
  Shape::Shape(),
  m_radius(1.0)
{
  m_params.center = startPoint;
  m_params.filled = filled;
  m_params.color = color;
  m_params.thickness = thickness;
  
  m_vertex_count = 32;
  m_vertices = new vec4[m_vertex_count];
  GLfloat theta = 0.0;
  GLfloat stepSize = 2.0*M_PI/m_vertex_count;
  for ( int i = 0; i < m_vertex_count; ++i )
  {
    m_vertices[i] = vec4(cos(theta)+m_params.center.x,
                         sin(theta)+m_params.center.y,
                         0.0,1.0);
    theta+=stepSize;
  }

  setScale(vec2(0.00001,0.00001));

  if ( m_params.filled ) fillShape();
  else                   unFillShape();
}

Circle::Circle(const Circle& rhs) :
  Shape::Shape(rhs),
  m_radius(rhs.m_radius)
{}

const Circle& Circle::operator=(const Circle& rhs)
{
  Shape::operator=(rhs);
  m_radius = rhs.m_radius;
  return *this;
}

void Circle::fillShape()
{
  m_shader.drawMode = GL_TRIANGLE_FAN;
  m_params.filled = true;
}

void Circle::unFillShape()
{
  m_shader.drawMode = GL_LINE_LOOP;
  m_params.filled = false;
}

bool Circle::isInside(vec2 loc) const
{
  if ( !m_shader.initialized ) return false;
  
  // bounding box check first
  mat4 modelViewInverse = getModelView(true);

  // get mouse location in model coordinates
  vec4 mouseLoc = modelViewInverse*vec4(loc.x,loc.y,0,1);
  vec2 mouseLoc2 = vec2(mouseLoc.x, mouseLoc.y) - m_params.center;

  return ( sqrt(mouseLoc2.x*mouseLoc2.x+mouseLoc2.y*mouseLoc2.y) <= m_radius );
}

void Circle::mouseDownChild(vec2 cameraCoordLoc, int mode)
{}

void Circle::mouseMoveChild(vec2 cameraCoordLoc, int mode)
{
  switch (mode)
  {
    case MODE_DRAW_CIRCLE:
      vec2 len = cameraCoordLoc - m_params.center;
      GLfloat scl = sqrt(len.x*len.x+len.y*len.y);
      setScale(vec2(scl,scl));
  }
}

void Circle::mouseUpChild(vec2 cameraCoordLoc, int mode)
{}

void Circle::Save(std::ofstream &fout) const
{
  // don't save uninitialized shapes
  if ( !m_shader.initialized )
    return;

  fout << "C" << std::endl;
  fout << m_params.center.x << ' ' << m_params.center.y << std::endl;
  fout << m_params.translate.x << ' ' << m_params.translate.y << std::endl;
  fout << m_params.theta << std::endl;
  fout << m_params.scale.x << ' ' << m_params.scale.y << std::endl;
  fout << m_params.color.x << ' ' << m_params.color.y << ' ' 
       << m_params.color.z << ' ' << m_params.color.w << std::endl;
  fout << m_params.thickness << std::endl;
  fout << m_params.filled << std::endl;

  fout << m_radius;
}

void Circle::Load(std::ifstream &fin, GLuint program)
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

  fin >> m_radius;

  m_vertex_count = 32;
  m_vertices = new vec4[m_vertex_count];
  GLfloat theta = 0.0;
  GLfloat stepSize = 2.0*M_PI/m_vertex_count;
  for ( int i = 0; i < m_vertex_count; ++i )
  {
    m_vertices[i] = vec4(m_radius*cos(theta)+m_params.center.x,
                         m_radius*sin(theta)+m_params.center.y,
                         0.0,1.0);
    theta+=stepSize;
  }

  if ( m_params.filled ) fillShape();
  else                   unFillShape();

  init(program);
}

