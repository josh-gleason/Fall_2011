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

  // shallow copies are okay no copy constructor or operator=

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
    initialized(false),
    program(0),
    vao(0),
    vbo(0),
    vPosition(0),
    center(0),
    translate(0),
    scale(0),
    color(0),
    drawMode(GL_TRIANGLES)
  {}

  // shallow copies are okay no copy constructor or operator=

  bool initialized;

  GLuint program;
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

    Shape(const Shape& rhs);
    const Shape& operator=(const Shape& rhs);

    void reset();
    void init(GLuint program);
    void draw() const;
   
    // modifiers (rotation/scaling always occur around the center)
    void rotate(GLfloat theta);
    void translate(const vec2& translation);
    void translate(GLfloat transX, GLfloat transY);
    void scale(const vec2& scale);
    void scale(GLfloat scaleX, GLfloat scaleY);
    void setColor(const vec4& color);
    void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

    // accessors
    GLfloat getTheta() const;
    const vec2& getTranslation() const;
    const vec2& getScale() const;
    const vec2& getCenter() const;  // returns center+translate
    const vec4& getColor() const;
    const ShapeParameters& getParams() const;

    // auxilary functions (virtual)
    virtual void keyPress(unsigned int key, vec2 loc);
    virtual void selectShape(int value);
    
    // project the mouse coords through the inverse transform of the object then
    // it should be easy to test if it's inside the original bounding box
    virtual bool isInside(vec2 loc);

  protected:
    ShapeParameters m_params;
    ShaderValues    m_shader;
    vec4*           m_vertices;
    int             m_vertex_count;
    
    // reset shape called by reset with def_values=true
    void resetShape(bool def_values=true);
};

#endif // _JDG_SHADE_CLASS_

