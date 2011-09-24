/** @file  Rect.h
  * @brief Defines the square class which inherits from the Poly<vec2> class.
  */

#ifndef __JDG_RECT_H__
#define __JDG_RECT_H__

#include "Poly.h"

/** @breif Rect class */
class Rect : public Poly<vec2>
{
public:
  Rect();
  Rect(const vec2& upper_left, const vec2& lower_right,
    const vec4& color=vec4(0.0,0.0,0.0,1.0));
  Rect(const Rect& rhs);
  ~Rect();

  virtual int vertex_count() const;
  virtual int vertex_dim() const;
  virtual GLenum get_mode() const;

  void setLocation(const vec2& upper_left, const vec2& lower_right);
  void setColor(const vec4& color);

  Rect& operator=(const Rect& rhs);
private:
  static const int VERTEX_COUNT = 6;
  static const int DIMS = 2;
};

#endif // __JDG_RECT_H__
