#include <iomanip>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>

#include "mat.h"
#include "Shape.h"
#include "InitShader.h"

using namespace std;

unsigned int WIN_HEIGHT = 500;
unsigned int WIN_WIDTH = 500;

GLuint program;

// Should contain all the shapes in the program
struct ShapePtr
{
  ShapePtr() : p(NULL) {}
  ~ShapePtr() { if (p != NULL) delete p; }
  
  Shape* p;
};

ShapePtr shape[2];

void init()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  program = InitShader("../src/shaders/vshader.glsl",
                       "../src/shaders/fshader.glsl");

  vec4 verts[6] = {
    vec4(-0.5,0.5,0,1),
    vec4(0.5,0.5,0,1),
    vec4(-0.5,-0.5,0,1),

    vec4(-0.5,-0.5,0.5,1),
    vec4(0.5,0.5,0.5,1),
    vec4(0.5,-0.5,0.5,1) };

  shape[0].p = new Shape(6, verts);
  shape[0].p->init(program);
  
  shape[0].p->rotate(-45);
  //shape[0].p->scale(0.5);
  //shape[0].p->translate(vec2(0));

  glClearColor(0.5,0.5,0.5,1.0);
}

void display()
{
  glClear( GL_COLOR_BUFFER_BIT );

  shape[0].p->draw();

  GLfloat projection[16], modelview[16];

  glGetFloatv(GL_TRANSPOSE_PROJECTION_MATRIX, projection);
  glGetFloatv(GL_TRANSPOSE_MODELVIEW_MATRIX, modelview);

  for ( int i = 0; i < 16; ++i )
  {
    cout << setw(8) << projection[i];
    if ( (i+1)%4 == 0 )
      cout << " )\n";
    else
      cout << ", ";
  }
  cout << endl;
  
  for ( int i = 0; i < 16; ++i )
  {
    cout << setw(8) << modelview[i];
    if ( (i+1)%4 == 0 )
      cout << " )\n";
    else
      cout << ", ";
  }
  cout << endl;

  glutSwapBuffers();
}

void resize(int width, int height)
{
  glViewport(0,0,width,height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(
    -1.0, // left
    1.0+((GLfloat)width-(GLfloat)WIN_WIDTH)/(WIN_WIDTH*0.5),        // right
    -(1.0+((GLfloat)height-(GLfloat)WIN_HEIGHT)/(WIN_HEIGHT*0.5)),  // bottom
    1.0   // top
  );

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
  //glutMouseFunc(mousePress);

  glutMainLoop();

  return 0;
}
