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
    Poly(const vec2& startPoint,
            bool filled=true,
            const vec4& color=vec4(0.0,0.0,0.0,1.0),
            GLfloat thickness=1.0);

    Poly(const Poly& rhs);
    const Poly& operator=(const Poly& rhs);

    void fillShape();
    void unFillShape();
    
    bool isInside(vec2 loc) const;

    void mouseDown(vec2 cameraCoordLoc, int mode);
    void mouseMove(vec2 cameraCorrdLoc, int mode);
    void mouseUp(vec2 cameraCorrdLoc, int mode);
    
    // takes current list of vertices assuming it represents a line loop
    // and triangulates the points.  Changes m_vertex_count and m_vertices
    // and assumes that m_outline has been set.  Calls Shape::reset(false)
    void triangulateVertices();
  protected:

    // holds the outline for the polygon
    std::vector<vec2> m_outline;
};

#endif // _JDG_SHAPE_POLYGON
