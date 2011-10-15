#ifndef _JDG_RECT_CLASS_
#define _JDG_RECT_CLASS__

#include "Shape.h"

class Rect : public Shape
{
  public:
    Rect();
    Rect(const vec2& upperLeft,
         const vec2& lowerRight,
         bool filled,
         const vec4& color=vec4(0.0,0.0,0.0,1.0));

    Rect(const Rect& rhs);
    const Rect& operator=(const Rect& rhs);
    vec2 getSize() const;
  void fillShape();
  void unFillShape();
  protected:

    vec2 m_upperLeft, m_lowerRight;
};

#endif
