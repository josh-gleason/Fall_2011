#include "Paddle.h"
#include "Ball.h"

using namespace std;

// Timer settings
const int TIMER_INTERVAL = 10;        // Timer step size
const int RESET_TIME = 100;           // number of frames to wait after ball resets

// Board settings
const GLfloat TOP_WALL = 1.0;         // location of top wall
const GLfloat BOTTOM_WALL = -1.0;     // location of bottom wall

// Ball settings
const GLfloat BALL_RADIUS = 0.04;     // radius of ball
const GLfloat Y_ACCEL = 0.0022;       // Y acceleration due to hitting a moving paddle
const GLfloat X_ACCEL = 0.0003;       // X acceleration every time the paddle is hit
const GLfloat MAX_SPEED = 0.05;       // Max speed the ball can move in x or y dir
const GLfloat START_SPEED = 0.007;     // starting speed of the ball (+-50% of this)

const GLfloat BALL_MAX_Y = 0.9;
const GLfloat BALL_MIN_Y = -0.9;
const GLfloat PADDLE_MAX_Y = 1.0;
const GLfloat PADDLE_MIN_Y = -1.0;

Paddle left_paddle, right_paddle;
Rect board;
Ball ball;

const GLfloat PADDLE_STEP = 0.02;

bool AI_enabled = true;
bool paused = false;

GLsizei width = 600, height = 600;

GLfloat mouseY = 0.0;

GLfloat ballX = 0, ballY = 0;
GLfloat ballDx = 0.01, ballDy = 0.005;

int resetBall = RESET_TIME; // start ball in 20 frames

mat4 proj_mat;

int score[2] = {0,0};

bool gameOver()
{
  return false; // TODO
}

vec2 randVelocity()
{
  GLfloat ballDx = (rand() % 200 + 100.0) * START_SPEED/200.0;
  GLfloat ballDy = (rand() % 200 + 100.0) * START_SPEED/200.0;

  if ( rand() % 2 == 0 )
    ballDx *= -1;
  if ( rand() % 2 == 0 )
    ballDy *= -1;

  if ( abs(ballDy) > abs(ballDx) )
    swap(ballDx,ballDy);

  return vec2(ballDx, ballDy);
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
 
void setPaddleDir( Paddle& paddle, GLfloat referenceY )
{
  GLfloat paddle_loc = paddle.getCenterY();
  
  if ( referenceY > paddle_loc+paddle.getStepSize() )  // move up
    paddle.move_flag = Paddle::UP;
  else if ( referenceY < paddle_loc-paddle.getStepSize() ) // move down
    paddle.move_flag = Paddle::DOWN; 
  else
    paddle.move_flag = Paddle::STILL;
}

void timer(int val)
{
  if ( paused )
    return;

  // always move the paddle towards the ball or the mouse
  if ( AI_enabled )
    setPaddleDir(right_paddle, ball.getCenterY());
  else
    setPaddleDir(right_paddle, mouseY);

  // move paddles
  int goal;

  right_paddle.takeStep();
  left_paddle.takeStep();
  goal = ball.takeStep(left_paddle, right_paddle);
  
  if ( goal != Ball::NO_GOAL )
  {
    ball.reset(vec2(0.0,0.0), randVelocity(), RESET_TIME);
    if ( goal == Ball::SCORE_LEFT )
      score[0] += 1;
    else  // SCORE_RIGHT
      score[1] += 1;
  }

  glutPostRedisplay();
  
  if ( !gameOver() )
    glutTimerFunc(TIMER_INTERVAL,timer,val);
}

void reshape(GLsizei w, GLsizei h)
{
  GLfloat x, y;

  glViewport(0,0,w,h);

  // set the projection matrix to a 2D orthographic projetion
  if ( h < w )
    proj_mat = Ortho2D(-1.0,1.0,-1.0*h/w,1.0*h/w);
  else
    proj_mat = Ortho2D(-1.0*w/h,1.0*w/h,-1.0,1.0);

  // copy projection matrix to each object
  left_paddle.setProjection(proj_mat);
  right_paddle.setProjection(proj_mat);
  ball.setProjection(proj_mat);

  glutPostRedisplay();

  width = w;
  height = h;
}

void init()
{
  // intialize the paddles
  left_paddle = 
    Paddle(
      vec2(-0.03,0.2),
      vec2(0.03,-0.2),
      vec4(0.8,0.2,0.2,1.0),
      PADDLE_STEP,
      PADDLE_MIN_Y, // bottom
      PADDLE_MAX_Y   // top
  );

  right_paddle = 
    Paddle(
      vec2(-0.03,0.2),
      vec2(0.03,-0.2),
      vec4(0.8,0.2,0.2,1.0),
      PADDLE_STEP,
      PADDLE_MIN_Y, // bottom
      PADDLE_MAX_Y   // top
  );
  
  // move to desired location
  left_paddle.setTranslation(-0.95,0);
  right_paddle.setTranslation(0.95,0);

  // intitialize the ball
  
  // random direction with speeds from 1/2 START_SPEED to START_SPEED

  ball = 
    Ball(
      vec2(0,0),
      BALL_RADIUS,
      vec4(0.2,0.2,0.8,1.0),
      randVelocity(),
      BALL_MIN_Y, // bottom
      BALL_MAX_Y,  // top
      -1.0, // left 
      1.0   // right
    );

  ball.pause_count = RESET_TIME;

  left_paddle.init();
  right_paddle.init();
  ball.init();

  glClearColor(0.2,0.8,0.2,1.0);
}

void display()
{
  glClear( GL_COLOR_BUFFER_BIT );
  
  // draw everything
  left_paddle.draw();
  right_paddle.draw();
  ball.draw();

  glutSwapBuffers();
}

int main(int argc, char *argv[])
{
  srand(time(0));
  //resetDxDy();

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
