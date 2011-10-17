#ifndef _JDG_SHAPE_POLYGON_
#define _JDG_SHAPE_POLYGON_

#include "Shape.h"

class Polygon : public Shape
{
  public:
    Polygon();
    Polygon(const vec2* vertices,
            int vertex_count,
            bool filled=true,
            const vec4& color=vec4(0.0,0.0,0.0,1.0),
            GLfloat thickness=1.0);
    Polygon(const vec2& startPoint,
            bool filled=true,
            const vec4& color=vec4(0.0,0.0,0.0,1.0),
            GLfloat thickness=1.0);

    Polygon(const Polygon& rhs);
    const Polygon& operator=(const Polygon& rhs);

    vec2 getSize() const;
    void fillShape();
    void unFillShape();
    
    bool isInside(vec2 loc) const;

    void mouseDown(vec2 cameraCoordLoc, int mode);
    void mouseMove(vec2 cameraCorrdLoc, int mode);
    void mouseUp(vec2 cameraCorrdLoc, int mode);
};

#endif // _JDG_SHAPE_POLYGON
