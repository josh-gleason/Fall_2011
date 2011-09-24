#include "Rect.h"
#include "Circle.h"

using namespace std;

Rect left_paddle;
Rect right_paddle;
Circle ball;
Rect board;

mat4 proj_mat;

void reshape(GLsizei w, GLsizei h)
{
  GLfloat x, y;

  glViewport(0,0,w,h);

  // set the projection matrix to a 2D orthographic projetion
  if ( h > w )
    proj_mat = Ortho2D(-1.0,1.0,-1.0*h/w,1.0*h/w);
  else
    proj_mat = Ortho2D(-1.0*w/h,1.0*w/h,-1.0,1.0);
  
  glutPostRedisplay();
}

void init()
{
  // set initial locations
  board =
    Rect(
      vec2(-0.9,0.9),
      vec2(0.9,-0.9),
      vec4(0.2,0.8,0.2,1.0)
  );

  left_paddle = 
    Rect(
      vec2(-0.8,0.3),
      vec2(-0.7,-0.3),
      vec4(0.8,0.2,0.2,1.0)
  );

  right_paddle = 
    Rect(
      vec2(0.7,0.3),
      vec2(0.8,-0.3),
      vec4(0.8,0.2,0.2,1.0)
  );

  ball =
    Circle(
      vec2(0.0,0.0),
      0.05,
      vec4(0.2,0.2,0.8,1.0)
  );
  
  left_paddle.init();
  right_paddle.init();
  ball.init();
  board.init();

  glClearColor(1.0,1.0,1.0,1.0);
}

void display()
{
  glClear( GL_COLOR_BUFFER_BIT );
  
  // draw everything
  board.draw(proj_mat);
  left_paddle.draw(proj_mat);
  right_paddle.draw(proj_mat);
  ball.draw(proj_mat);

  glutSwapBuffers();
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(512,512);
  
  glutCreateWindow("Pong");

  glewInit();
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  glutMainLoop();

  return 0;
}
