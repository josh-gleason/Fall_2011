#include "Circle.h"

Circle::Circle()
{
  vertices = new vec2[VERTEX_COUNT];
  colors = new vec4[VERTEX_COUNT];

  setLocation(vec2(0.0,0.0),0.1);
  setColor(vec4(0.0,0.0,0.0,1.0));
} 

Circle::Circle(const vec2& center, GLfloat radius,
  const vec4& color)
{
  vertices = new vec2[VERTEX_COUNT];
  colors = new vec4[VERTEX_COUNT];

  setLocation(center, radius);
  setColor(color);
}

Circle::Circle(const Circle& rhs)
{
  vertices = new vec2[VERTEX_COUNT];
  colors = new vec4[VERTEX_COUNT];

  for ( int i = 0; i < VERTEX_COUNT; ++i )
  {
    vertices[i] = rhs.vertices[i];
    colors[i] = rhs.colors[i];
  }
}

Circle::~Circle()
{
  delete [] vertices;
  delete [] colors;
}

void Circle::setLocation(const vec2& center, GLfloat radius)
{
  // set first and last vertex and first center
  vertices[0] = center; 

  for ( int i = 0; i < CIRC_POINTS; ++i )
  {
    GLfloat theta = i/(CIRC_POINTS/2.0)*M_PI;
    
    vertices[i+1] = vec2(
        center.x+radius*cos(theta),
        center.y+radius*sin(theta)
    );
  }

  vertices[CIRC_POINTS+1] = vertices[1];
}

void Circle::setColor(const vec4& color)
{
  // set all vertices to same color
  for ( int i = 0; i < VERTEX_COUNT; ++i )
    colors[i] = color;
}

Circle& Circle::operator=(const Circle& rhs)
{
  for ( int i = 0; i < VERTEX_COUNT; ++i )
  {
    vertices[i] = rhs.vertices[i];
    colors[i] = rhs.colors[i];
  }
  return *this;
}

int Circle::vertex_count() const
{
  return VERTEX_COUNT;
}

int Circle::vertex_dim() const
{
  return DIMS;
}

GLenum Circle::get_mode() const
{
  return GL_TRIANGLE_FAN;
}
