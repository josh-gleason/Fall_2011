#ifndef _JDG_SHAPE_CLASS_
#define _JDG_SHAPE_CLASS_

#include "vec.h"

struct ShapeParameters
{
  ShapeParameters() :
    center(0,0,0),
  	translate(0.0),
  	rotate(0.0),
  	scale(1.0),
  	color(0.0,0.0,0.0,1.0),
  	filled(false),
  	selected(false)
  {}

  vec3 center;
  vec3 translate;
  vec3 rotate;
  vec3 scale;

  vec4 color;
  
  bool filled;
  bool selected;
};

struct ShaderValues
{
  ShaderValues() :
    vao(0),
    vbo(0),
    vPosition(0),
    uColor(0),
    drawMode(GL_TRIANGLES)
  {}

  GLuint vao;
  GLuint vbo;
  GLuint vPosition;
  GLuint uColor;

  GLenum drawMode;
};

class Shape
{
  public:
    Shape();
    Shape(int numVertecies, const vec4* vertices=NULL);
    ~Shape();

    void draw();
    void init(GLuint program);
    
    void rotate(GLfloat theta);
    void translate(vec2 translation);
    void scale(vec2 scale);

    void keyPress(unsigned int key, vec2 loc);
    void selectShape(int value);

    const ShapeParameters& Params() const;
  private:
    ShapeParameters m_params;
    ShaderValues    m_shader;
    vec4*           m_vertices;
    int             m_vertex_count;
};

#endif // _JDG_SHADE_CLASS_

