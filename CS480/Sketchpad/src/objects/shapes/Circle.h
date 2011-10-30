
#ifndef _JDG_CIRCLE_CLASS_
#define _JDG_CIRCLE_CLASS__

#include "Shape.h"

class Circle : public Shape
{
  public:
    Circle();
    Circle(const vec2& center,
           GLfloat radius,
           bool filled,
           const vec4& color=vec4(0.0,0.0,0.0,1.0),
           GLfloat thickness=1.0);

    Circle(const vec2& startPoint,
           bool filled=true,
           const vec4& color=vec4(0.0,0.0,0.0,1.0),
           GLfloat thickness=1.0);

    Circle(const Circle& rhs);
    const Circle& operator=(const Circle& rhs);

    void fillShape();
    void unFillShape();
    
    bool isInside(vec2 loc) const;

    void Save(std::ofstream &fout) const;
    void Load(std::ifstream &fin, GLuint program);
  protected:
    GLfloat m_radius;

    void mouseDownChild(vec2 cameraCoordLoc, int mode);
    void mouseMoveChild(vec2 cameraCoordLoc, int mode);
    void mouseUpChild(vec2 cameraCoordLoc, int mode);
};

#endif // _JDG_CIRCLE_CLASS_
