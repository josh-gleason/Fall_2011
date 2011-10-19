#ifndef _JDG_SHAPE_POLYGON_
#define _JDG_SHAPE_POLYGON_

#include <vector>
#include "Shape.h"

class Poly : public Shape
{
  public: 
    Poly();
    Poly(const vec2* vertices,
            int vertex_count,
            bool filled=true,
            const vec4& color=vec4(0.0,0.0,0.0,1.0),
            GLfloat thickness=1.0);

    Poly(const Poly& rhs);
    Poly(const vec2& startPoint,
         bool filled=true,
         const vec4& color=vec4(0.0,0.0,0.0,1.0),
         GLfloat thickness=1.0);

    ~Poly();
    const Poly& operator=(const Poly& rhs);

    void fillShape();
    void unFillShape();
    
    bool isInside(vec2 loc) const;

    void mouseDown(vec2 cameraCoordLoc, int mode);
    void mouseMove(vec2 cameraCorrdLoc, int mode);
    void mouseUp(vec2 cameraCorrdLoc, int mode);
  protected: 
    vec4* m_vertices_filled;
    vec4* m_vertices_outline;
    unsigned m_vertex_count_filled;
    unsigned m_vertex_count_outline;
};

#endif // _JDG_SHAPE_POLYGON
