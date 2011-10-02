#include "Rect.h"

Rect::Rect()
{
  vertices = new vec2[VERTEX_COUNT];
  colors = new vec4[VERTEX_COUNT];

  setLocation(vec2(-1.0,-1.0),vec2(1.0,1.0));
  setColor(vec4(0.0,0.0,0.0,1.0),false);
} 

Rect::Rect(const vec2& upper_left, const vec2& lower_right,
  const vec4& color)
{
  vertices = new vec2[VERTEX_COUNT];
  colors = new vec4[VERTEX_COUNT];

  setLocation(upper_left, lower_right);
  setColor(color,false);
}

Rect::Rect(const Rect& rhs)
{
  vertices = new vec2[VERTEX_COUNT];
  colors = new vec4[VERTEX_COUNT];

  setLocation(rhs.vertices[0],rhs.vertices[5]);
  setColor(rhs.colors[0],false);
}

Rect::~Rect()
{
  delete [] vertices;
  delete [] colors;
}

void Rect::setLocation(const vec2& upper_left, const vec2& lower_right)
{
  vec2 upper_right(lower_right.x, upper_left.y),
       lower_left(upper_left.x, lower_right.y);

  // build rectangle out of two triangles
  vertices[0] = upper_left;
  vertices[1] = vertices[3] = upper_right;
  vertices[2] = vertices[4] = lower_left;
  vertices[5] = lower_right;
}

Rect& Rect::operator=(const Rect& rhs)
{
  for ( int i = 0; i < VERTEX_COUNT; ++i )
  {
    vertices[i] = rhs.vertices[i];
    colors[i] = rhs.colors[i];
  }
  return *this;
}

int Rect::vertex_count() const
{
  return VERTEX_COUNT;
}

int Rect::vertex_dim() const
{
  return DIMS;
}

GLenum Rect::get_mode() const
{
  return GL_TRIANGLES;
}
