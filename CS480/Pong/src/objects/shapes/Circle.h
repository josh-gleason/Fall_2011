/** @file  Circle.h
  * @brief Defines the circle class which inherits from the Poly<vec2> class.
  */

#ifndef __JDG_CIRCLE_H__
#define __JDG_CIRCLE_H__

#include "Poly.h"

/** @breif Circle class */
class Circle : public Poly<vec2>
{
public:
  Circle();
  Circle(const vec2& center, GLfloat radius,
         const vec4& color=vec4(0.0,0.0,0.0,1.0),
         int circlePoints=32);   ///<@ must be multiple of 4
  Circle(const Circle& rhs);
  ~Circle();

  virtual int vertex_count() const;
  virtual int vertex_dim() const;
  virtual GLenum get_mode() const;

  void setLocation(const vec2& center, GLfloat radius);

  Circle& operator=(const Circle& rhs);
protected:
  // number of points defining the perimeter of the circle
  int circle_points;
  static const int DIMS = 2;
};

#endif // __JDG_CIRCLE_H__
