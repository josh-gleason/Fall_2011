#include <iomanip>
#include <fstream>
#include <iostream>
#include <sstream>
#include <list>

#include "mat.h"
#include "Shape.h"
#include "Rect.h"
#include "InitShader.h"

using namespace std;

const char VSHADER_NAME[] = "../src/shaders/vshader.glsl";
const char FSHADER_NAME[] = "../src/shaders/fshader.glsl";

unsigned int WIN_HEIGHT = 500;
unsigned int WIN_WIDTH = 500;

mat4 projectionMatrix;
list<Shape> shapes;

// global shader indecies
GLuint program;
GLuint projection;

void init()
{
  program = InitShader(VSHADER_NAME, FSHADER_NAME);
  projection = glGetUniformLocation(program,"projection");

  shapes.push_back(Rect(vec2(-1,1), vec2(1,-1), true, vec4(0.8,0.2,0.2,1.0)));
  shapes.back().init(program);

  shapes.back().scale(0.2,0.25);
  shapes.back().rotate(100);
  shapes.back().translate(-0.4,-0.6);

  glClearColor(0.5,0.5,0.5,1.0);
}

void display()
{
  glClear( GL_COLOR_BUFFER_BIT );

  // set the projections matrix
  glUniformMatrix4fv(projection,1,GL_TRUE,projectionMatrix);

  for ( list<Shape>::iterator i = shapes.begin(); i != shapes.end(); ++i )
    i->draw();

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

void mousePress(int button, int state, int x, int y)
{
  if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
  {
    vec2 cameraCoords = mouse2Camera(x, y);

    if ( shapes.back().isInside(cameraCoords) )
      cout << "Inside" << endl;
  }
  else if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
  {
    shapes.back().toggleFilled();
  }
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
  //glutMotionFunc(mouseActiveMotion);
  //glutPassiveMotionFunc(mousePassiveMotion); 
  glutMouseFunc(mousePress);

  glutMainLoop();

  return 0;
}
