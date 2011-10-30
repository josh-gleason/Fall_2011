#ifndef _JDG_POINT_CLASS_
#define _JDG_POINT_CLASS_

#include "Shape.h"

class Point : public Shape
{
public:
  Point();
  Point(const vec2& loc, const vec4& color, GLfloat thickness);
  Point(const Point& rhs);

  bool isInside(vec2 loc) const;

  void Save(std::ofstream &fout) const;
  void Load(std::ifstream &fin, GLuint program);
};

#endif // _JDG_POINT_CLASS_
