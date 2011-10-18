#ifndef _JDG_SHAPE_POLYGON_
#define _JDG_SHAPE_POLYGON_

#include "Shape.h"

// used for triangulation
#include <del_interface.hpp>

class Poly : public Shape
{
  public: 
    // used for tesselation
    struct PolyData{
      ShapeParameters* params;
      ShaderValues* shader;
      //vec4* vertices;
      //int vertex_count;
      std::vector<vec2> verts;
    };

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
    std::vector<vec2> m_outline;
/*
    void beginCallback( GLenum type );
    void endCallback();
    void vertexCallback( void *vertexData );
    void combineCallback(GLdouble coords[3],
                         GLdouble *vertex_data[4],
                         GLfloat weight[4],
                         void **outData);
*/
};

#endif // _JDG_SHAPE_POLYGON
