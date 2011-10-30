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
#include "Point.h"

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

// used for translate rotate and scale
vec3 point1(0.0,0.0,-1.0), point2(0.0,0.0,-1.0), point3(0.0,0.0,-1.0);

vec2 A, B, C;
mat2 Rot_J, Scale_J;

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

  params.thickness = 3.0;
  params.color = vec4(0.0,0.0,0.0,1.0);

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
    if ( selectedIndex == index )
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

void selectObject(vec2 coords)
{
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


        ///////////////////// Add new types here for selection /////////////
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
        else if ( typeid(*shapes[index]) == typeid(Point) )
        {
          vec2 uL, lR;
          uL = vec2(shapes[index]->getVertices()[0].x,
                    shapes[index]->getVertices()[0].y) +
                    shapes[index]->getTranslation() +
               vec2(-0.01,0.01);
          lR = vec2(shapes[index]->getVertices()[0].x,
                    shapes[index]->getVertices()[0].y) +
                    shapes[index]->getTranslation() +
               vec2(0.01,-0.01);

          selectedShape = ShapePtr(new Rect(uL,lR,true));
          selectedShape->init(program);
        }
        ////////////////////////////////////////////////////////////////////

        if ( selectedIndex >= 0 )
          shapes[selectedIndex]->unSelectShape();
        shapes[index]->selectShape();

        if ( typeid(*shapes[index]) != typeid(Point) )
        {
          selectedShape->unFillShape();
          if ( shapes[selectedIndex]->getFilled() )
            selectedShape->setThickness(20.0);
          else
            selectedShape->setThickness(
              shapes[selectedIndex]->getThickness()+19.0);
        }

        selectedShape->setColor(colorInv(shapes[index]->getColor()));
        glutPostRedisplay();

      }
      break;
    }

  } while ( index != start );
  if ( index == start && !shapes[index]->isInside(coords) )
    selectedIndex = -1;
}

float getAngle(vec2 center, vec2 pt1, vec2 pt2)
{
  // uses the law of cosines
  float lenC1, len12, lenC2;
  lenC1 = sqrt((center.x-pt1.x)*(center.x-pt1.x)+
               (center.y-pt1.y)*(center.y-pt1.y));
  len12 = sqrt((pt1.x-pt2.x)*(pt1.x-pt2.x)+
               (pt1.y-pt2.y)*(pt1.y-pt2.y));
  lenC2 = sqrt((center.x-pt2.x)*(center.x-pt2.x)+
               (center.y-pt2.y)*(center.y-pt2.y));
 
  float angle = ( std::acos((lenC1*lenC1-len12*len12+lenC2*lenC2) /
                  (2.0*lenC1*lenC2)) * 180.0/M_PI );

  // cross product to figure out clockwise or counter-clockwise
  if ( cross(pt1-center,pt2-center).z > 0 ) 
    return angle;
  else
    return -angle;
}

void mouseDown(vec2 coords)
{
  mousePressed = true;

  switch (programMode)
  {
    case MODE_DRAW_RECT:
      drawingShape = ShapePtr(
          new Rect(coords, params.filled, params.color, params.thickness));
      drawingShape->init(program);
      drawingShape->mouseDown(coords, programMode);
      break;
    case MODE_DRAW_LINE_SEG:
    case MODE_DRAW_POLY:
      if ( !drawingShape )
      {
        cout << "Building shape" << endl;
        drawingShape = ShapePtr(
            new LineSegs(coords, params.color, params.thickness));
        drawingShape->init(program);
      }
      drawingShape->mouseDown(coords, programMode);
      break;
    case MODE_DRAW_POINT:
    {
      ShapePtr p = ShapePtr(new Point(coords, params.color, params.thickness));
      shapes.push_back(p);
      shapes.back()->init(program);
      break;
    }
    case MODE_SELECT:
      selectObject(coords);
      break;
  }

  // must be a shape selected for these to work
  if ( selectedIndex >= 0 )
  {
    switch (programMode)
    {
      case MODE_TRANSLATE_SHAPE:
      {
        vec2 trans = shapes[selectedIndex]->getTranslation();
        point3 = vec3(trans.x,trans.y,0.0);
        point1 = vec3(coords.x,coords.y,0.0);
        break;
      }
      case MODE_ROTATE_SHAPE:
      {
        if ( typeid(*shapes[selectedIndex]) == typeid(Point) )
          break;
        GLfloat theta = shapes[selectedIndex]->getTheta();
        vec2 center = shapes[selectedIndex]->getCenter();
        point3 = vec3(center.x,center.y,theta);
        point1 = vec3(coords.x,coords.y,0.0);
        break;
      }
      case MODE_SCALE_SHAPE:
      {
        if ( typeid(*shapes[selectedIndex]) == typeid(Point) )
          break;
        GLfloat theta = -shapes[selectedIndex]->getTheta()*M_PI/180.0;
        
        vec2 scale = shapes[selectedIndex]->getScale();

        C = shapes[selectedIndex]->getCenter();
        
        Rot_J = mat2(cos(theta),-sin(theta),
                   sin(theta), cos(theta));

        Scale_J = mat2(scale.x,0,
                     0,      scale.y);

        A = Rot_J*(coords-C);

//        point3 = vec3(center.x,center.y,cos(theta));
//        point2 = vec3(scale.x,scale.y,sin(theta));
//        point1 = vec3(coords.x-center.x,coords.y-center.y,0.0);
//        point1 = vec3(point1.x*point3.z-point1.y*point2.z,
//                      point1.y*point2.z-point1.x*point3.z,theta);
        break;
      }
    }
  }

}

void mouseMove(vec2 coords)
{
  switch (programMode)
  {
    case MODE_DRAW_RECT:
    case MODE_DRAW_LINE_SEG:
    case MODE_DRAW_POLY:
      if ( !drawingShape )
        return;
      drawingShape->mouseMove(coords, programMode);
      break;
  }

  if ( selectedIndex >= 0 )
  {
    switch (programMode)
    {
      case MODE_TRANSLATE_SHAPE:
      {
        point2 = vec3(coords.x,coords.y,0.0);
        vec3 newTrans = point3+point2-point1;
        shapes[selectedIndex]->setTranslate(vec2(newTrans.x,newTrans.y));
        selectedShape->setTranslate(vec2(newTrans.x,newTrans.y));
        break;
      }
      case MODE_ROTATE_SHAPE:
      {
        if ( typeid(*shapes[selectedIndex]) == typeid(Point) )
          break;
        point2 = vec3(coords.x,coords.y,0.0);
        GLfloat newTheta = point3.z+getAngle(
            vec2(point3.x,point3.y),
            vec2(point1.x,point1.y),
            vec2(point2.x,point2.y));
        shapes[selectedIndex]->setTheta(newTheta);
        selectedShape->setTheta(newTheta);
        break;
      }
      case MODE_SCALE_SHAPE:
      {
        if ( typeid(*shapes[selectedIndex]) == typeid(Point) )
          break;
        // point1 is the original clicked point
        // point2 is the original scale
        // point3 is the center of the object

        //vec2 firstVec = vec2(point1.x,point1.y);
        //vec2 secondVec = coords-vec2(point3.x,point3.y);
       
        B = Rot_J*(coords-C)-A;

        //secondVec = vec2(secondVec.x*point3.z-secondVec.y*point2.z,
        //                 secondVec.x*point2.z+secondVec.y*point3.z);

        //secondVec = secondVec - firstVec;

        vec2 scale;
        
        scale.x = (B.x / A.x);
        scale.y = (B.y / A.y);

        scale = vec2(Scale_J[0][0]*scale.x,Scale_J[1][1]*scale.y)
                   + vec2(Scale_J[0][0],Scale_J[1][1]);

        shapes[selectedIndex]->setScale(scale);
        selectedShape->setScale(scale);
        break;
      }
    }
  }
}

void mouseUp(vec2 coords)
{
  mousePressed = false;
  
  switch (programMode)
  {
    case MODE_DRAW_RECT:
      if ( !drawingShape )
        return;
      drawingShape->mouseUp(coords, programMode);
      shapes.push_back(drawingShape);
      drawingShape = ShapePtr();
      break;
  }
}

void mouseMotion(int x, int y)
{
  if ( mousePressed || ( drawingShape && (
       programMode == MODE_DRAW_LINE_SEG ||
       programMode == MODE_DRAW_POLY ) ) )
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
  {
    mouseDown(coords);
    glutPostRedisplay();
  }
  else if ( button == GLUT_LEFT_BUTTON && state == GLUT_UP )
  {
    mouseUp(coords);
    glutPostRedisplay();
  }

  if ( button == GLUT_RIGHT_BUTTON && drawingShape )
  {
    if ( programMode == MODE_DRAW_LINE_SEG )
    {
      shapes.push_back(drawingShape);
      drawingShape = ShapePtr();
      glutPostRedisplay();
    }
    else if ( programMode == MODE_DRAW_POLY )
    {
      // convert the LineSeg into a Poly
      ShapePtr p =
        ShapePtr(new Poly(*reinterpret_cast<LineSegs*>(&(*drawingShape))));
      shapes.push_back(p);
      drawingShape = ShapePtr();
      glutPostRedisplay();
    }
  }
}


void keyboardPress(unsigned char key, int x, int y)
{
  // keyboard disabled when mouse is pressed
  if ( mousePressed ) return;

  if ( !drawingShape  )
  {
    switch (key)
    {
      // change of mode cases
      case 'r': // draw rectangle
        if ( programMode == MODE_DRAW_RECT )
          return;
        clearSelection();
        programMode = MODE_DRAW_RECT;
        break;
      case 'p': // draw polygon
        if ( programMode == MODE_DRAW_POLY )
          return;
        clearSelection();
        programMode = MODE_DRAW_POLY;
        break;
      case 'c': // draw circle
        if ( programMode == MODE_DRAW_CIRCLE )
          return;
        clearSelection();
        programMode = MODE_DRAW_CIRCLE;
        break;
      case 'g': // draw line segment
        if ( programMode == MODE_DRAW_LINE_SEG )
          return;
        clearSelection();
        programMode = MODE_DRAW_LINE_SEG;
        break;
      case 'o': // draw points
        if ( programMode == MODE_DRAW_POINT )
          return;
        clearSelection();
        programMode = MODE_DRAW_POINT;
        break;
      case 's': // select mode
        if ( programMode == MODE_SELECT )
          return;
        cout << "SELECT MODE" << endl;
        programMode = MODE_SELECT;
        break;
    }
  }

  switch (key)
  {
    case 27:  // esc key
      if ( !mousePressed )
      {
        clearSelection();
        drawingShape = ShapePtr();
        programMode = MODE_NONE;
        glutPostRedisplay();
      }
      break;
  }

  if ( selectedIndex >= 0 )
  {
    switch (key)
    {
      case 't': // translate mode
        programMode = MODE_TRANSLATE_SHAPE;
        cout << "TRANSLATE MODE" << endl;
        break;
      case 'y': // scale mode
        programMode = MODE_SCALE_SHAPE;
        cout << "SCALE MODE" << endl;
        break;
      case 'u': // rotate mode
        programMode = MODE_ROTATE_SHAPE;
        cout << "ROTATE MODE" << endl;
        break;
      case 'l': // color mode
        programMode = MODE_CHANGE_COLOR;
        break;
      case 'm': // toggle filled
        shapes[selectedIndex]->toggleFilled();
        if ( shapes[selectedIndex]->getFilled() )
          selectedShape->setThickness(20.0);
        else
          selectedShape->setThickness(
            shapes[selectedIndex]->getThickness()+19.0);
        glutPostRedisplay();
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
  glutMouseFunc(mousePress);
  glutPassiveMotionFunc(mouseMotion); 
  glutMotionFunc(mouseMotion);

  glutMainLoop();

  return 0;
}
