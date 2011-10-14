#ifndef _JDG_SHAPE_CLASS_
#define _JDG_SHAPE_CLASS_

#include "vec.h"

struct ShapeParameters
{
  ShapeParameters() :
    center(0.0,0.0),
  	translate(0.0),
  	theta(0.0),
  	scale(1.0,1.0),
  	color(0.0,0.0,0.0,1.0),
  	filled(false),
  	selected(false)
  {}

  vec2 center;
  vec2 translate;
  GLfloat theta;
  vec2 scale;

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
    center(0),
    translate(0),
    scale(0),
    color(0),
    drawMode(GL_TRIANGLES)
  {}

  GLuint vao;
  GLuint vbo;

  // attribute variables
  GLuint vPosition;
  
  // uniform variables
  GLuint center;
  GLuint translate;
  GLuint theta;
  GLuint scale;
  GLuint color;

  GLenum drawMode;
};

class Shape
{
  public:
    Shape();
    Shape(int numVertecies,
          const vec4* vertices=NULL,
          const vec4& color=vec4(0.0,0.0,0.0,1.0),
          const vec2& center=vec2(0.0,0.0));
    ~Shape();

    void draw();
    void init(GLuint program);
    
    void rotate(GLfloat theta);
    void translate(vec2 translation);
    void scale(vec2 scale);

    virtual void keyPress(unsigned int key, vec2 loc);
    virtual void selectShape(int value);
    virtual bool isInside(vec2 loc);

    const ShapeParameters& Params() const;
  private:
    ShapeParameters m_params;
    ShaderValues    m_shader;
    vec4*           m_vertices;
    int             m_vertex_count;
};

#endif // _JDG_SHADE_CLASS_

