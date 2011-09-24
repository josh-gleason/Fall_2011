#include "InitShader.h"
#include <iostream>

using namespace std;

int NumVertices;

void divideTriangle(GLfloat *points, int len)
{
  // end condition
  if ( len <= 3 )
    return;

  len /= 3;

  GLfloat orig[6] = {
    points[0], points[1],
    points[2], points[3],
    points[4], points[5] };

  // crazy shit that I made up in my head
  for ( int i = 0; i < 3; ++i )
  {
    // take midpoint of each vertex
    points[2*i*len]=orig[2*i];
    points[2*i*len+1]=orig[2*i+1];
    points[2*(1+i*len)]=(orig[2*i]+orig[2*((i+1)%3)])/2;
    points[2*(1+i*len)+1]=(orig[2*i+1]+orig[2*((i+1)%3)+1])/2;
    points[2*(2+i*len)]=(orig[2*i]+orig[2*((i+2)%3)])/2;
    points[2*(2+i*len)+1]=(orig[2*i+1]+orig[2*((i+2)%3)+1])/2;
    
    divideTriangle(&points[2*len*i], len);
  }
}

void init()
{
  srand(time(0));

  GLfloat* points = new GLfloat[2*NumVertices];
  GLfloat* colors = new GLfloat[4*NumVertices];

  int points_size = sizeof(GLfloat)*(2*NumVertices);
  int colors_size = sizeof(GLfloat)*(4*NumVertices);
  int total_size = points_size + colors_size;

  // A triangle in the plane z = 0
  //points[0] = -0.5-rand()%101/200.0; points[1] = -0.5-rand()%101/200.0;
  //points[2] = -0.25+rand()%101/200.0;  points[3] = 0.5+rand()%101/200.0;
  //points[4] = 0.5+rand()%101/200.0;  points[5] = -0.5-rand()%101/200.0;
 
  points[0] = -1; points[1] = -1;
  points[2] = 0; points[3] = 1;
  points[4] = 1; points[5] = -1;

  divideTriangle(points,NumVertices);

  // set colors in each section
  for ( int i = 0; i < 3; ++i )
  {
    static const int Third = NumVertices/3; 
    for ( int j = 0; j < Third; ++j )
    {
      colors[Third*i*4+3+j*4]=1.0;
      for ( int k = 0; k < 3; ++k )
        if ( i == k )
          colors[Third*i*4+k+j*4] = 0.5+(rand()%128)/255.0;
        else
          colors[Third*i*4+k+j*4] = (rand()%128)/255.0;
    }
  }

  // load shaders
  GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
  glUseProgram( program );
  
  // Create vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers(1,&buffer);
  glBindBuffer(GL_ARRAY_BUFFER,buffer);
  glBufferData(GL_ARRAY_BUFFER, total_size, NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, points_size, points);
  glBufferSubData(GL_ARRAY_BUFFER, points_size, colors_size, colors);
  
  // Initialize the vertex position attribute from the vertex shader
  GLuint vPosition = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  GLuint vColor= glGetAttribLocation(program, "vColor");
  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(points_size) );

  glClearColor( 0.0, 0.0, 0.0, 1.0 );
  delete [] points;
  delete [] colors;
}

void display()
{
  glClear( GL_COLOR_BUFFER_BIT );
  glDrawArrays( GL_TRIANGLES, 0, NumVertices );
  glFlush();
}

int main(int argc, char *argv[])
{
  int NumLevels = 5;
  if ( argc > 1 )
    NumLevels = max(min(atoi(argv[1]),10),0);
  
  // set global value for number of vertices
  NumVertices = pow(3,NumLevels+1);

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

