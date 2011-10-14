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

const char VSHADER_NAME[] = "../src/shaders/vshader.glsl";
const char FSHADER_NAME[] = "../src/shaders/fshader.glsl";

unsigned int WIN_HEIGHT = 500;
unsigned int WIN_WIDTH = 500;

GLuint program;
GLuint projection;

mat4 projectionMatrix;

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

  program = InitShader(VSHADER_NAME, FSHADER_NAME);
  projection = glGetUniformLocation(program,"projection");

  vec4 verts[6] = {
    vec4(-1,1,0,1),
    vec4(1,1,0,1),
    vec4(-1,-1,0,1),

    vec4(-1,-1,0,1),
    vec4(1,1,0,1),
    vec4(1,-1,0,1) };

  shape[0].p = new Shape(6, verts, vec4(0.8,0.2,0.2,1.0), vec2(0.0,0.0));
  shape[0].p->init(program);
 
  shape[1].p = new Shape(6, verts, vec4(0.2,0.8,0.2,1.0), vec2(0.0,0.0));
  shape[1].p->init(program);

  shape[0].p->rotate(-45);
  shape[0].p->scale(0.25);
  shape[0].p->translate(vec2(0.5));

  shape[1].p->rotate(30);
  shape[1].p->scale(1.0/6.0);
  shape[1].p->translate(vec2(1.0,-1.5));

  glClearColor(0.5,0.5,0.5,1.0);
}

void display()
{
  glClear( GL_COLOR_BUFFER_BIT );

  // set the projections matrix
  glUniformMatrix4fv(projection,1,GL_TRUE,projectionMatrix);

  shape[0].p->draw();
  shape[1].p->draw();

  glutSwapBuffers();
}

void resize(int width, int height)
{
  glViewport(0,0,width,height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  projectionMatrix = Ortho2D(
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
