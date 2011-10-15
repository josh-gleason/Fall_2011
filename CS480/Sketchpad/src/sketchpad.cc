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

list<Shape> shapes;

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

  shapes.push_back(Shape(6, verts, vec4(0.2,0.8,0.2,1.0), vec2(0.0,0.0)));
  shapes.back().init(program);

  shapes.back().scale(0.01);
  shapes.back().translate(2.0,-1.25);

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

void mousePress(int button, int state, int x, int y)
{
  if ( button != GLUT_RIGHT_BUTTON || state != GLUT_DOWN ) return;
  vector<list<Shape>::iterator> shape_its(shapes.size());

  int index = 0;
  for ( list<Shape>::iterator i = shapes.begin();i != shapes.end(); ++i )
    shape_its[index++] = i;

  for ( size_t i = 0; i < shape_its.size(); ++i )
  {
    shapes.push_back(*shape_its[i]);
    shapes.back().translate(vec2((rand()%255)/(255.0)*0.5*((rand()%2)*2-1),(rand()%255)/(255.0)*0.5*((rand()%2)*2-1)));
    shapes.back().setColor(vec4((rand()%256)/(255.0),(rand()%256)/(255.0),(rand()%256)/(255.0),1.0));
    shapes.back().rotate(rand() % 90);
    shape_its[i]->translate(vec2((rand()%255)/(255.0)*0.5*((rand()%2)*2-1),(rand()%255)/(255.0)*0.5*((rand()%2)*2-1)));
    shape_its[i]->rotate(rand() % 90);
  }

  std::cout << shapes.size() << std::endl;


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
