#ifndef _JDG_SHAPE_CLASS_
#define _JDG_SHAPE_CLASS_

#include "mat.h"

struct ShapeParameters
{
  ShapeParameters() :
    center(0.0,0.0),
  	translate(0.0),
  	theta(0.0),
  	scale(1.0,1.0),
  	color(0.0,0.0,0.0,1.0),
    thickness(1.0),
  	filled(true),
  	selected(false)
  {}

  // shallow copies are okay no copy constructor or operator=

  vec2 center;
  vec2 translate;
  GLfloat theta;
  vec2 scale;

  vec4 color;
  
  GLfloat thickness;  // used only if not filled

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
    Shape(int numVertices,
          GLenum drawMode,
          const vec4* vertices=NULL,
          const vec4& color=vec4(0.0,0.0,0.0,1.0),
          const vec2& center=vec2(0.0,0.0));
    virtual ~Shape();

    Shape(const Shape& rhs);
    virtual const Shape& operator=(const Shape& rhs);

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
    void setThickness(GLfloat);
    void setFilled(bool filled);
    void toggleFilled();

    // accessors
    GLfloat getTheta() const;
    const vec2& getTranslation() const;
    const vec2& getScale() const;
    const vec2 getCenter() const;  // returns center+translate
    const vec4& getColor() const;
    GLfloat getThickness() const;
    bool getFilled() const;
    int getVertexCount() const;

    GLenum getDrawMode() const;

    const ShapeParameters& getParams() const;

    // returns 2x2 matrix with
    // [ minX minY ]
    // [ maxX maxY ]
    mat2 getBoundingBox() const;

    // builds and returns the model-view matrix or it's inverse
    mat4 getModelView(bool inverseTranspose = false) const;
    
    void mouseDown(vec2 cameraCoordLoc, int mode);
    void mouseMove(vec2 cameraCoordLoc, int mode);  // must be called after mouseDown
    void mouseUp(vec2 cameraCoordLoc, int mode);    // must be called after mouseMove

    // auxilary functions (virtual)
    virtual void toggleSelectShape(int value=0);
    virtual void selectShape(int value=0);
    virtual void unSelectShape(int value=0);
    
    // project the mouse coords through the inverse transform of the object then
    // it should be easy to test if it's inside the original bounding box
    virtual bool isInside(vec2 loc) const;

    virtual void fillShape();
    virtual void unFillShape();
  protected:
    // called by mouseDown and MouseMove and MouseUp
    virtual void mouseDownChild(vec2 cameraCoordLoc, int mode);
    virtual void mouseMoveChild(vec2 cameraCoordLoc, int mode);
    virtual void mouseUpChild(vec2 cameraCoordLoc, int mode);
    
    ShapeParameters m_params;
    ShaderValues    m_shader;
    vec4*           m_vertices;
    int             m_vertex_count;
    
    // allows 3 global mouse coords to be used for mouse events
    vec2 mouseCoords[3];

    // reset shape called by reset with defvalues=true
    void resetShape(bool defaultValues=true);
    void setScale(const vec2& scaling);
};

#endif // _JDG_SHADE_CLASS_

