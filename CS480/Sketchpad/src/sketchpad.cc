#include <iomanip>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "mat.h"

#include "Rect.h"
#include "Poly.h"
#include "LineSegs.h"

#include "InitShader.h"
#include "SketchPadDefs.h"

using namespace std;

const char VSHADER_NAME[] = "../src/shaders/vshader.glsl";
const char FSHADER_NAME[] = "../src/shaders/fshader.glsl";

unsigned int WIN_HEIGHT = 500;
unsigned int WIN_WIDTH = 500;

typedef boost::shared_ptr<Shape> ShapePtr;

mat4 projectionMatrix;
vector<ShapePtr> shapes;

ShapePtr drawingShape;

// used when drawing shapes, some parameters may not apply to all objects
ShapeParameters params;


// the current mode of the program
int programMode = DRAW_RECT;  // TODO: probably shouldn't be default mode

// global shader indecies
GLuint program;
GLuint projection;

// tells the program if the mouse is being held down
bool mousePressed = false;

// TODO: implement an unProject function to avoid deprecated glu function

void init()
{
  program = InitShader(VSHADER_NAME, FSHADER_NAME);
  projection = glGetUniformLocation(program,"projection");

// TODO temp ///////////////////////////////////////////////////////////////
  
  // generate random polygon
  const int VERT_NUMBER = 100;
  vec2 pts[VERT_NUMBER];

  for( int i = 0; i < VERT_NUMBER; ++i )
    pts[i] = vec2((rand()%501)/500.0*1.5-0.75,
                  (rand()%501)/500.0*1.5-0.75);

  ShapePtr p(new Poly(pts,VERT_NUMBER,false,vec4(0.2,0.2,0.8,1.0)));
  shapes.push_back(p);
  shapes.back()->init(program);
  
  shapes.back()->scale(1.2,1.2);
  shapes.back()->translate(0.25,-0.25);
  shapes.back()->rotate(-45);

  p = ShapePtr(new LineSegs(pts,VERT_NUMBER,vec4(0.8,0.2,0.2,1.0),3.0));
  shapes.push_back(p);
  shapes.back()->init(program);
  
  shapes.back()->scale(0.7,0.7);
  shapes.back()->translate(-0.25,0.25);
  shapes.back()->rotate(60);
////////////////////////////////////////////////////////////////////////////

  glClearColor(0.5,0.5,0.5,1.0);
}

void display()
{
  glClear( GL_COLOR_BUFFER_BIT );

  // set the projections matrix
  glUniformMatrix4fv(projection,1,GL_TRUE,projectionMatrix);

  for ( vector<ShapePtr>::iterator i = shapes.begin(); i != shapes.end(); ++i )
    (*i)->draw();

  if ( drawingShape )
    drawingShape->draw();

  glutSwapBuffers();
}

void resize(int width, int height)
{
  glViewport(0,0,width,height);
  projectionMatrix = Ortho2D(
    -(1.0+(width-(GLfloat)WIN_WIDTH)/WIN_WIDTH),  // left
    1.0+(width-(GLfloat)WIN_WIDTH)/WIN_WIDTH,       // right
    -(1.0+(height-(GLfloat)WIN_HEIGHT)/WIN_HEIGHT),  // bottom
    1.0+(height-(GLfloat)WIN_HEIGHT)/WIN_HEIGHT     // top
  );

  glutPostRedisplay();
}

vec2 mouse2Camera(int x, int y)
{
  const mat4* m = &projectionMatrix;

  GLdouble proj[16] =
    {(*m)[0][0], (*m)[1][0], (*m)[2][0], (*m)[3][0],
     (*m)[0][1], (*m)[1][1], (*m)[2][1], (*m)[3][1],
     (*m)[0][2], (*m)[1][2], (*m)[2][2], (*m)[3][2],
     (*m)[0][3], (*m)[1][3], (*m)[2][3], (*m)[3][3]};

  static GLdouble model[16] = {1.0, 0.0, 0.0, 0.0,
                               0.0, 1.0, 0.0, 0.0,
                               0.0, 0.0, 1.0, 0.0,
                               0.0, 0.0, 0.0, 1.0};
  
  GLdouble mouseX = static_cast<GLdouble>(x);
  GLdouble mouseY = static_cast<GLdouble>(y);
  
  GLdouble objX, objY, objZ;
  GLint view[4];

  glGetIntegerv(GL_VIEWPORT, view);

  gluUnProject(mouseX, mouseY, 0, model, proj, view,
              &objX, &objY, &objZ);

  // for some reason objY comes out negative
  return vec2(static_cast<GLfloat>(objX),
              static_cast<GLfloat>(-objY));
}

void mouseDown(vec2 coords)
{
  mousePressed = true;

  switch (programMode)
  {
    case DRAW_RECT:
      drawingShape = ShapePtr(
        new Rect(coords, params.filled, params.color, params.thickness));
      drawingShape->init(program);
      drawingShape->mouseDown(coords, programMode);
      break;
  }
}

void mouseMove(vec2 coords)
{
  switch (programMode)
  {
    case DRAW_RECT:
      drawingShape->mouseMove(coords, programMode);
      break;
  }
}

void mouseUp(vec2 coords)
{
  mousePressed = false;
  
  switch (programMode)
  {
    case DRAW_RECT:
      drawingShape->mouseUp(coords, programMode);
      shapes.push_back(drawingShape);
      drawingShape = ShapePtr();
      break;
  }
}

void mouseMotion(int x, int y)
{
  if ( mousePressed )
  {
    vec2 coords = mouse2Camera(x,y);
    mouseMove(coords);
    glutPostRedisplay();
  }
}

void mousePress(int button, int state, int x, int y)
{
  vec2 coords = mouse2Camera(x,y);

  if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    mouseDown(coords);
  else if ( button == GLUT_LEFT_BUTTON && state == GLUT_UP )
    mouseUp(coords);

  // TODO: temp ////////////////////////////////////////////////////////////
  if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
  {
    shapes.back()->toggleFilled();
  /*
    params.color = vec4((rand()%256)/255.0, (rand()%256)/255.0, (rand()%256)/255.0, 1.0);
    params.filled = ((rand()%2)==1);
    params.thickness = (rand()%256)/255.0*9.0+1.0;
    cout << params.color << endl;
  */
  }

  if ( button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN )
  {
    if ( shapes.back()->isInside(coords) )
      std::cout << "Inside" << std::endl;
    else
      std::cout << "Outside" << std::endl;
  }
  //////////////////////////////////////////////////////////////////////////
  
  glutPostRedisplay();
}

int main(int argc, char *argv[])
{
  srand(time(0));
  
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
  glutCreateWindow("Sketchpad");

  glewInit();
  init();

  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  //glutKeyboardFunc(keyboardPress);

  //glutKeyboardUpFunc(keyboardUp);
  //glutPassiveMotionFunc(mousePassiveMotion); 
  glutMouseFunc(mousePress);
  glutMotionFunc(mouseMotion);

  glutMainLoop();

  return 0;
}
