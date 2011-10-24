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
int programMode = MODE_NONE;

int selectedIndex = -1;
ShapePtr selectedShape;

// global shader indecies
GLuint program;
GLuint projection;

// tells the program if the mouse is being held down
bool mousePressed = false;

// TODO: implement an unProject function to avoid deprecated glu function

vec4 colorInv(vec4 input)
{
  return vec4(1.0,1.0,1.0,2.0)-input;
}

void init()
{
  program = InitShader(VSHADER_NAME, FSHADER_NAME);
  projection = glGetUniformLocation(program,"projection");

// TODO temp ///////////////////////////////////////////////////////////////
  
  // generate random polygon
  /*const int VERT_NUMBER = 100;
  vec2 pts[VERT_NUMBER+1];

  for( int i = 0; i < VERT_NUMBER; ++i )
    pts[i] = vec2((rand()%501)/500.0*1.5-0.75,
                  (rand()%501)/500.0*1.5-0.75);
  pts[VERT_NUMBER]=pts[0];
  */
  ShapePtr p;
  /*p = ShapePtr(new Poly(pts,VERT_NUMBER,false,vec4(0.2,0.2,0.8,1.0)));
  shapes.push_back(p);
  shapes.back()->init(program);
  
  shapes.back()->scale(2);
  shapes.back()->rotate(60);
  
  p = ShapePtr(new LineSegs(pts,VERT_NUMBER+1,vec4(0.8,0.2,0.2,1.0),10.0));
  shapes.push_back(p);
  shapes.back()->init(program);
  
  shapes.back()->scale(2);
  shapes.back()->rotate(60);
  */
  p = ShapePtr(new Rect(vec2(-0.5,0.5),vec2(0.5,-0.5),false,vec4(0.0,0.0,0.0,1.0),1.0));
  shapes.push_back(p);
  shapes.back()->init(program);

  shapes.back()->unFillShape();
  shapes.back()->setThickness(2.0);
  shapes.back()->setColor(vec4(1.0,1.0,0.0,1.0));
////////////////////////////////////////////////////////////////////////////

  glClearColor(0.5,0.5,0.5,1.0);
}

void display()
{
  glClear( GL_COLOR_BUFFER_BIT );

  // set the projections matrix
  glUniformMatrix4fv(projection,1,GL_TRUE,projectionMatrix);

  int index = 0;
  for ( vector<ShapePtr>::iterator i = shapes.begin(); i != shapes.end(); ++i )
  {
    if ( selectedIndex == index && selectedIndex >= 0 )
      selectedShape->draw();
    (*i)->draw();
    index++;
  }
    
  if ( drawingShape )
    drawingShape->draw();

  glutSwapBuffers();
}

void clearSelection()
{
  if ( selectedIndex >= 0 )
  {
    selectedIndex = -1;
    glutPostRedisplay();
  }
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
    case MODE_DRAW_RECT:
      drawingShape = ShapePtr(
// TODO        new Rect(coords, params.filled, params.color, params.thickness));
        new Rect(coords, false, params.color, params.thickness));
      drawingShape->init(program);
      drawingShape->mouseDown(coords, programMode);
      break;
    case MODE_SELECT:
      unsigned start = 0;
      if ( selectedIndex >= 0 )
        start = selectedIndex;
      
      unsigned index = start;
      do {
        if ( index == 0 )
          index = shapes.size();
        index--;
        if ( shapes[index]->isInside(coords) )
        {
          if ( selectedIndex != index )
          {
            selectedIndex = index;
            if      ( typeid(*shapes[index]) == typeid(Rect) )
            {
              cout << "Selected" << endl;
              selectedShape = ShapePtr(new
                Rect(*(reinterpret_cast<Rect*>(&(*shapes[index])))));
            }
            else if ( typeid(*shapes[index]) == typeid(Poly) )
              selectedShape = ShapePtr(new
                Poly(*(reinterpret_cast<Poly*>(&(*shapes[index])))));
            else if ( typeid(*shapes[index]) == typeid(LineSegs) )
              selectedShape = ShapePtr(new
                LineSegs(*(reinterpret_cast<LineSegs*>(&(*shapes[index])))));
            
            if ( selectedIndex >= 0 )
              shapes[selectedIndex]->unSelectShape();
            shapes[index]->selectShape();

            selectedShape->unFillShape();
            if ( shapes[selectedIndex]->getFilled() )
              selectedShape->setThickness(20.0);
            else
              selectedShape->setThickness(
                shapes[selectedIndex]->getThickness()+19.0);
            selectedShape->setColor(colorInv(shapes[index]->getColor()));
            glutPostRedisplay();

            cout << selectedShape->getThickness() << endl;
          
          }
          cout << selectedIndex << endl;
          break;
        }

      } while ( index != start );

  }
}

void mouseMove(vec2 coords)
{
  switch (programMode)
  {
    case MODE_DRAW_RECT:
      drawingShape->mouseMove(coords, programMode);
      break;
  }
}

void mouseUp(vec2 coords)
{
  mousePressed = false;
  
  switch (programMode)
  {
    case MODE_DRAW_RECT:
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
    shapes.back()->toggleFilled();

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


void keyboardPress(unsigned char key, int x, int y)
{
  if ( programMode == MODE_NONE )
  {
    switch (key)
    {
      // change of mode cases
      case 'r': // draw rectangle
        clearSelection();
        programMode = MODE_DRAW_RECT;
        break;
      case 'p': // draw polygon
        clearSelection();
        programMode = MODE_DRAW_POLY;
        break;
      case 'c': // draw circle
        clearSelection();
        programMode = MODE_DRAW_CIRCLE;
        break;
      case 'g': // draw line segment
        clearSelection();
        programMode = MODE_DRAW_LINE_SEG;
        break;
      
      case 's': // select mode
        clearSelection();
        programMode = MODE_SELECT;
        break;
      case 'l': // color mode
        if ( selectedIndex >= 0 )
          programMode = MODE_CHANGE_COLOR;
        break;
    }
  }
  else
  {
    switch (key)
    {
      case 27:  // Escape TODO
        if ( !mousePressed )
        {
          clearSelection();
          programMode = MODE_NONE;
          cout << "Reset Program Mode" << endl;
        }
        else
          cout << "Mouse pressed?" << endl;
        break;
    }
  }
}


//// glut special function keys
// GLUT_KEY_F1   F1 function key
// GLUT_KEY_F2   F2 function key
// GLUT_KEY_F3   F3 function key
// GLUT_KEY_F4   F4 function key
// GLUT_KEY_F5   F5 function key
// GLUT_KEY_F6   F6 function key
// GLUT_KEY_F7   F7 function key
// GLUT_KEY_F8   F8 function key
// GLUT_KEY_F9   F9 function key
// GLUT_KEY_F10    F10 function key
// GLUT_KEY_F11    F11 function key
// GLUT_KEY_F12    F12 function key
// GLUT_KEY_LEFT   Left function key
// GLUT_KEY_RIGHT    Right function key
// GLUT_KEY_UP   Up function key
// GLUT_KEY_DOWN   Down function key
// GLUT_KEY_PAGE_UP  Page Up function key
// GLUT_KEY_PAGE_DOWN  Page Down function key
// GLUT_KEY_HOME   Home function key
// GLUT_KEY_END    End function key
// GLUT_KEY_INSERT   Insert function key
void specialPress(int key, int x, int y)
{

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
  glutKeyboardFunc(keyboardPress);
  glutSpecialFunc(specialPress);

  //glutKeyboardUpFunc(keyboardUp);
  //glutPassiveMotionFunc(mousePassiveMotion); 
  glutMouseFunc(mousePress);
  glutMotionFunc(mouseMotion);

  glutMainLoop();

  return 0;
}
