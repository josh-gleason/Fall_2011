#ifndef _JDG_SHAPES_LINESEGS_
#define _JDG_SHAPES_LINESEGS_

#include "Shape.h"

// class prototype so we can be friends
class Poly;

class LineSegs : public Shape
{
  public:
    LineSegs();

    // the vertices parameter assumes each vertex represents the next line
    // segment which is connect to the tail of the previous line segment.
    LineSegs(const vec2* vertices,
             int vertex_count,
             const vec4& color=vec4(0.0,0.0,0.0,1.0),
             GLfloat thickness=1.0);

    LineSegs(const vec2& startPoint,
             const vec4& color=vec4(0.0,0.0,0.0,1.0),
             GLfloat thickness=1.0);

    LineSegs(const LineSegs& rhs);
    const LineSegs& operator=(const LineSegs& rhs);

    // These functions do nothing b/c line segs are open
    // void fillShape();
    // void unFillShape();

    bool isInside(vec2 loc) const;

    void Save(std::ofstream &fout) const;
    void Load(std::ifstream &fin,GLuint program);

    friend class Poly;
  protected:

    void mouseDownChild(vec2 cameraCoordLoc, int mode);
    void mouseMoveChild(vec2 cameraCoordLoc, int mode);
    void mouseUpChild(vec2 cameraCoordLoc, int mode);    
};

#endif // _JDG_SHAPES_LINESEGS_
