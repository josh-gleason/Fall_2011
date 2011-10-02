#include "Rect.h"
#include "Circle.h"
#include "Paddle.h"

using namespace std;

const int TIMER_INTERVAL = 10;
const GLfloat TOP_WALL = 1.0;
const GLfloat BOTTOM_WALL = -1.0;
const int RESET_TIME = 100;
const GLfloat BALL_RADIUS = 0.04;
const GLfloat Y_ACCEL = 0.0022;
const GLfloat X_ACCEL = 0.0003;

const GLfloat MAX_SPEED = 0.05;

const GLfloat START_SPEED = 0.01;

Paddle left_paddle, right_paddle;
Circle ball;

mat4 proj_mat;

GLsizei width = 512, height = 512;

GLfloat mouseY = 0.0;

GLfloat ballX = 0, ballY = 0;
GLfloat ballDx = 0.01, ballDy = 0.005;

int resetBall = RESET_TIME; // start ball in 20 frames

void resetDxDy()
{
  // random direction with speeds from 1/2 START_SPEED to START_SPEED
  ballDx = (rand() % 100 + 100.0) * START_SPEED/200.0;
  ballDy = (rand() % 100 + 100.0) * START_SPEED/200.0;

  if ( rand() % 2 == 0 )
    ballDx *= -1;
  if ( rand() % 2 == 0 )
    ballDy *= -1;

  if ( abs(ballDy) > abs(ballDx) )
    swap(ballDx,ballDy);
}

void keypress( unsigned char key, int x, int y )
{
  switch (key)
  {
    case 'w':
    case 'W':  
      left_paddle.move_flag += 1; 
      break;
    case 's':
    case 'S':
      left_paddle.move_flag -= 1;
      break;
  }
}

void keyup( unsigned char key, int x, int y )
{
  switch (key)
  {
    case 'w':
    case 'W':
      left_paddle.move_flag -= 1;
      break;
    case 's':
    case 'S':
      left_paddle.move_flag += 1; 
      break;
  }
}

void mousemove( int left, int top )
{
  // convert to world coords
  if ( width >= height )
    mouseY = 2.0/height*(height/2.0-top);
  else
    mouseY = 2.0/width*(height/2.0-top);
}

void timer(int val)
{
  GLfloat paddle_loc = (right_paddle.topY()+right_paddle.bottomY())/2.0;

  //cout << mouseY << ' ' << paddle_loc << endl;
  if ( mouseY > paddle_loc+right_paddle.getStepSize() )  // move up
    right_paddle.move_flag = Paddle::UP;
  else if ( mouseY < paddle_loc-right_paddle.getStepSize() ) // move down
    right_paddle.move_flag = Paddle::DOWN; 
  else
    right_paddle.move_flag = Paddle::STILL;

  bool lMove, rMove;
  // move paddles
  rMove = right_paddle.takeStep();
  lMove = left_paddle.takeStep();

  //ball.takeStep();
  if ( resetBall > 0 )
    resetBall--;
  else
  {
    ballX += ballDx;
    ballY += ballDy;
    
    // check top and bottom
    if ( ballY+BALL_RADIUS > TOP_WALL && ballDy > 0 )
    {
      ballDy *= -1;
      ballY += 2*ballDy;
    }
    else if ( ballY-BALL_RADIUS < BOTTOM_WALL && ballDy < 0 )
    { 
      ballDy *= -1;
      ballY += 2*ballDy;
    }

    if ( ballDx < 0 && // check left paddle
         ballX-BALL_RADIUS <= left_paddle.rightX() &&
         ballY-BALL_RADIUS <= left_paddle.topY()-BALL_RADIUS*0.5 &&
         ballY+BALL_RADIUS >= left_paddle.bottomY()+BALL_RADIUS*0.5 )
    {
      if ( ballX-BALL_RADIUS - ballDx > left_paddle.rightX() )
      {
        // check if paddle is moving
        if ( lMove && left_paddle.move_flag == Paddle::UP && abs(ballY) < MAX_SPEED )
          ballDy += Y_ACCEL;
        else if ( lMove && abs(ballY) < MAX_SPEED )
          ballDy -= Y_ACCEL;

        ballDx *= -1;
        ballX += 2*ballDx;
        
        if ( abs(ballDx) < MAX_SPEED )
          ballDx += (ballDx > 0 ? X_ACCEL : -X_ACCEL);
      }
      else  // already lost but ball should reflect off top/bot of paddle
      {
        // determine which direction ball should go by which end of paddle is
        // closer
        if ( abs(ballY - left_paddle.bottomY()) <
             abs(ballY - left_paddle.topY()) )
        {
          ballDy = -1*abs(ballDy);  // force down
          ballY = left_paddle.bottomY()-BALL_RADIUS;
        }
        else
        {
          ballDy = abs(ballDy);     // force up
          ballY = left_paddle.topY()+BALL_RADIUS;
        }
      }
    }
    else if ( ballDx > 0 && // check right paddle
         ballX+BALL_RADIUS >= right_paddle.leftX() &&
         ballY-BALL_RADIUS <= right_paddle.topY()-BALL_RADIUS*0.5 &&
         ballY+BALL_RADIUS >= right_paddle.bottomY()+BALL_RADIUS*0.5 )
    {  
      if ( ballX+BALL_RADIUS - ballDx < right_paddle.leftX() )
      {
        // check if paddle is moving
        if ( rMove && right_paddle.move_flag == Paddle::UP 
          && abs(ballDy) < MAX_SPEED )
          ballDy += Y_ACCEL;
        else if ( rMove && abs(ballDy) < MAX_SPEED ) // down
          ballDy -= Y_ACCEL;
        
        ballDx *= -1;
        ballX += 2*ballDx;
        
        // speed up the ball every hit
        if ( abs(ballDx) < MAX_SPEED )
          ballDx += (ballDx > 0 ? X_ACCEL : -X_ACCEL);
      }
      else
      {
        // determine which direction ball should go by which end of paddle is
        // closer
        if ( abs(ballY - right_paddle.bottomY()) <
             abs(ballY - right_paddle.topY()) )
        {
          ballDy = -1*abs(ballDy);  // force down
          ballY = right_paddle.bottomY()-BALL_RADIUS;
        }
        else
        {
          ballDy = abs(ballDy);     // force up
          ballY = right_paddle.topY()+BALL_RADIUS;
        }
      }
    }

    // check for lossing point
    if ( ballX-BALL_RADIUS > 1.0 )  // right side lost
    {
      //TODO add point to left
      resetBall = RESET_TIME;
      ballX = 0;
      ballY = 0;
      resetDxDy();
    }
    else if ( ballX+BALL_RADIUS < -1.0 ) // left side lost
    {
      //TODO add point to right
      resetBall = RESET_TIME;
      ballX = 0;
      ballY = 0;
      resetDxDy();
    }

  }

  glutPostRedisplay();
  
  glutTimerFunc(TIMER_INTERVAL,timer,val);
}

void reshape(GLsizei w, GLsizei h)
{
  GLfloat x, y;

  glViewport(0,0,w,h);

  // set the projection matrix to a 2D orthographic projetion
  if ( h > w )
    proj_mat = Ortho2D(-1.0,1.0,-1.0*h/w,1.0*h/w);
  else
    proj_mat = Ortho2D(-1.0*w/h,1.0*w/h,-1.0,1.0);

  // copy projection matrix to each object
  left_paddle.setProjection(proj_mat);
  right_paddle.setProjection(proj_mat);
  //ball.setProjection(proj_mat);

  glutPostRedisplay();

  width = w;
  height = h;
}

void init()
{
  // set initial locations (everthing is centered at 0,0)
  left_paddle = 
    Paddle(
      vec2(-0.03,0.2),
      vec2(0.03,-0.2),
      vec4(0.8,0.2,0.2,1.0)
  );

  right_paddle = 
    Paddle(
      vec2(-0.03,0.2),
      vec2(0.03,-0.2),
      vec4(0.8,0.2,0.2,1.0)
  );

  ball = 
    Circle(
      vec2(ballX,ballY),
      BALL_RADIUS,
      vec4(0.2,0.2,0.8,1.0)
    );

  left_paddle.setTranslation(-0.95,0);
  right_paddle.setTranslation(0.95,0);

  left_paddle.init();
  right_paddle.init();
  ball.init();

  glClearColor(0.2,0.8,0.2,1.0);
}

void display()
{
  glClear( GL_COLOR_BUFFER_BIT );
  
  // draw everything
  vec4 trans_vec(ballX,ballY,0.0,0.0);
  
  left_paddle.draw();
  right_paddle.draw();
  ball.draw(proj_mat, trans_vec);

  glutSwapBuffers();
}

int main(int argc, char *argv[])
{
  srand(time(0));
  resetDxDy();

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(width,height);
  
  glutCreateWindow("Pong");

  glewInit();
  init();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutTimerFunc(0,timer,0);

  glutKeyboardFunc(keypress);
  glutKeyboardUpFunc(keyup);
  
  // same function for both active and passive motion
  glutMotionFunc(mousemove);
  glutPassiveMotionFunc(mousemove);

  glutMainLoop();

  return 0;
}
