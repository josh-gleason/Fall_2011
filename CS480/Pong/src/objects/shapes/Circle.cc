#include "Circle.h"

Circle::Circle()
: circle_points(32)
{
  vertices = new vec2[(circle_points+2)];
  colors = new vec4[(circle_points+2)];

  setLocation(vec2(0.0,0.0),0.1);
  setColor(vec4(0.0,0.0,0.0,1.0),false);
} 

Circle::Circle(const vec2& center, GLfloat radius,
  const vec4& color, int circlePoints)
: circle_points(circlePoints)
{
  vertices = new vec2[(circle_points+2)];
  colors = new vec4[(circle_points+2)];

  setLocation(center, radius);
  setColor(color,false);

  circle_points = circlePoints;
}

Circle::Circle(const Circle& rhs)
{
  vertices = new vec2[(circle_points+2)];
  colors = new vec4[(circle_points+2)];

  for ( int i = 0; i < (circle_points+2); ++i )
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

  for ( int i = 0; i < circle_points; ++i )
  {
    GLfloat theta = i/(circle_points/2.0)*M_PI;
    
    vertices[i+1] = vec2(
        center.x+radius*cos(theta),
        center.y+radius*sin(theta)
    );
  }

  vertices[circle_points+1] = vertices[1];
}

Circle& Circle::operator=(const Circle& rhs)
{
  for ( int i = 0; i < (circle_points+2); ++i )
  {
    vertices[i] = rhs.vertices[i];
    colors[i] = rhs.colors[i];
  }
  return *this;
}

int Circle::vertex_count() const
{
  return circle_points+2;
}

int Circle::vertex_dim() const
{
  return DIMS;
}

GLenum Circle::get_mode() const
{
  return GL_TRIANGLE_FAN;
}
