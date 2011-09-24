#include "InitShader.h"

const int NumPoints = 5000;

void init()
{
  GLfloat points[NumPoints*2];

  // A triangle in the plane z = 0
  GLfloat vertices[6]={
    -1.0,-1.0,
    0.0,1.0,
    1.0,-1.0};

  // An arbitrary initial point inside the triangle
  points[0] = 0.25;
  points[1] = 0.50;

  // compute and store NumPoints-1 new points

  for ( int k = 1; k < NumPoints; k++ )
  {
    int j = rand() % 3;

    // compute the mid point between selected vertex
    // and previous point
    points[k*2] = (points[k*2-2]+vertices[j*2])/2.0;
    points[k*2+1] = (points[k*2-1]+vertices[j*2+1])/2.0;
  }

  // load shaders
  GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
  glUseProgram( program );

  // Create vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint buffer;
  glGenBuffers(1,&buffer);
  glBindBuffer(GL_ARRAY_BUFFER,buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  GLuint loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

void display()
{
  glClear( GL_COLOR_BUFFER_BIT );
  glDrawArrays( GL_POINTS, 0, NumPoints );
  glFlush();
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowSize(512,512);
  
  glutCreateWindow("Sierpinkski Gasket");

  glewInit();
  init();
  glutDisplayFunc(display);

  glutMainLoop();

  return 0;
}

