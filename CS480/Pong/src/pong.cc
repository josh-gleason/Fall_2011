#include "Paddle.h"
#include "Ball.h"
#include <sstream>

using namespace std;

// text font
const void* FONT = GLUT_BITMAP_TIMES_ROMAN_24;

// menu constants
const int MENU_RESTART_GAME = 0;
const int MENU_PAUSE = 1;
const int MENU_TOGGLE_AI = 2;
  
const int MENU_RED = 0;
const int MENU_BLACK = 1;
const int MENU_BLUE = 2;
const int MENU_ORANGE = 3;
const int MENU_CYAN = 4;

// game colors
const vec4 COLOR_RED(0.8,0.2,0.2,1.0);
const vec4 COLOR_BLACK(0.0,0.0,0.0,1.0);
const vec4 COLOR_BLUE(0.2,0.2,0.8,1.0);
const vec4 COLOR_ORANGE(0.8,0.4,0.1,1.0);
const vec4 COLOR_CYAN(0.2,0.8,0.8,1.0);
const vec4 COLOR_GREEN(0.2,0.8,0.2,1.0);
const vec4 COLOR_YELLOW(0.5,0.2,0.5,1.0);

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
const GLfloat START_SPEED = 0.009;    // starting speed of the ball (+-50% of this)

const GLfloat BALL_MAX_Y = 0.9;
const GLfloat BALL_MIN_Y = -0.9;
const GLfloat PADDLE_MAX_Y = 1.0;
const GLfloat PADDLE_MIN_Y = -1.0;

const GLfloat PADDLE_STEP = 0.02;

const int FLASH_TIMES = 30;           // number of time paddle flashes on win
const int FLASH_SPEED = 100;

const int resetBall = RESET_TIME;     // start ball in 20 frames

const int MAX_GOAL = 3;               // number of goals required to win the game

int winner = 0;

// should always hold the correct size of the window
GLsizei width = 600;
GLsizei height = 600;

// displayed objects
Paddle left_paddle, right_paddle;
Rect board;
Ball ball;

// Global flags
bool AI_enabled = true;
bool paused = false;

GLfloat mouseY = 0.0;
int score[2] = {0,0};

// holds the paddle color
vec4 paddleColor = COLOR_RED;

// projection matrix
mat4 proj_mat;

// function prototype
bool gameOver();

void writeString(GLfloat x, GLfloat y, string message)
{
  vec4 loc(x,y,0,0);
  loc = proj_mat*loc;
  glRasterPos2f(loc[0],loc[1]);
  for ( size_t i = 0; i < message.size(); ++i )
    glutBitmapCharacter(const_cast<void*>(FONT),message[i]);
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

void ballColorMenu(int value)
{
  switch (value)
  {
    case MENU_RED:
      ball.setColor(COLOR_RED);
      break;
    case MENU_BLACK:
      ball.setColor(COLOR_BLACK);
      break;
    case MENU_BLUE:
      ball.setColor(COLOR_BLUE);
      break;
    case MENU_ORANGE:
      ball.setColor(COLOR_ORANGE);
      break;
    case MENU_CYAN:
      ball.setColor(COLOR_CYAN);
      break;
  }
  glutPostRedisplay();
}

void paddleColorMenu(int value)
{
  switch (value)
  {
    case MENU_RED:
      paddleColor = COLOR_RED;
      break;
    case MENU_BLACK:
      paddleColor = COLOR_BLACK;
      break;
    case MENU_BLUE:
      paddleColor = COLOR_BLUE;
      break;
    case MENU_ORANGE:
      paddleColor = COLOR_ORANGE;
      break;
    case MENU_CYAN:
      paddleColor = COLOR_CYAN;
      break;
  }
  left_paddle.setColor(paddleColor);
  right_paddle.setColor(paddleColor);
  glutPostRedisplay();
}

void resetGame()
{
  score[0] = 0; score[1] = 0;
  left_paddle.setTranslation(-0.95,0);
  right_paddle.setTranslation(0.95,0);
  ball.reset(vec2(0.0,0.0), randVelocity(), RESET_TIME);
}

void popupMenu(int value)
{
  switch (value)
  {
    case MENU_RESTART_GAME:
      resetGame();
      break;
    case MENU_PAUSE:
      paused = !paused;
      break;
    case MENU_TOGGLE_AI:
      resetGame();
      AI_enabled = !AI_enabled;
      break;
  }
  glutPostRedisplay();
}

void timer(int val)
{
  if ( paused )
  {
    glutTimerFunc(TIMER_INTERVAL,timer,val);
    return;
  }

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
  
  if (!gameOver())   // check if the game is over
    glutTimerFunc(TIMER_INTERVAL,timer,val);
}


void winnerSequence(int value)
{
  if ( value <= 0 )
  {
    resetGame();
    winner = 0;
    glutTimerFunc(0,timer,0);
    return;
  }

  // flash the winning paddle
  Paddle* winner = (score[0] == MAX_GOAL?&left_paddle:&right_paddle);

  if ( value%2 == 0 )
    winner->setColor(COLOR_YELLOW);
  else
    winner->setColor(paddleColor);
 
  glutPostRedisplay();

  glutTimerFunc(FLASH_SPEED,winnerSequence,--value);
}

bool gameOver()
{
  if ( score[0] >= MAX_GOAL || score[1] >= MAX_GOAL )
  { 
    winner = (score[0] >= MAX_GOAL ? 1 : 2);
    glutTimerFunc(0,winnerSequence,FLASH_TIMES);
    return true;
  }
  return false;
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
    case ' ':
      paused = !paused;
      glutPostRedisplay();
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
  ball.setProjection(proj_mat);

  glutPostRedisplay();

  width = w;
  height = h;
}

void init()
{
  // set up the board
  board = 
    Rect(
      vec2(-1.0,0.9),
      vec2(1.0,-0.9),
      COLOR_GREEN);

  // intialize the paddles
  left_paddle = 
    Paddle(
      vec2(-0.03,0.2),
      vec2(0.03,-0.2),
      COLOR_RED,
      PADDLE_STEP,
      PADDLE_MIN_Y, // bottom
      PADDLE_MAX_Y   // top
  );

  right_paddle = 
    Paddle(
      vec2(-0.03,0.2),
      vec2(0.03,-0.2),
      COLOR_RED,
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

  board.init();
  left_paddle.init();
  right_paddle.init();
  ball.init();

  glClearColor(0.1,0.1,0.1,1.0);
}

void display()
{
  glClear( GL_COLOR_BUFFER_BIT );
  
  // draw everything
  board.draw(proj_mat,vec4());
  left_paddle.draw();
  right_paddle.draw();
  ball.draw();

  ostringstream player1_score;
  ostringstream player2_score;

  glColor3f(1.0,1.0,1.0);
  player1_score << "Player 1:\t" << score[0];
  if ( AI_enabled )
    player2_score << "Computer:\t" << score[1];
  else
    player2_score << "Player 2:\t" << score[1];


  if ( winner == 1 )
    writeString(-0.3,0.92,"Player 1 WINS!!!");
  else if ( winner == 2 )
  {
    if ( AI_enabled )
      writeString(-0.3,0.92,"Computer WINS!!!");
    else
      writeString(-0.3,0.92,"Player 2 WINS!!!");
  }

  writeString(-0.97,0.92,player1_score.str());
  writeString((AI_enabled?0.55:0.62),0.92,player2_score.str());

  glColor3f(0.3,0.3,0.3);
  if ( paused )
    writeString(-0.15,0.75,"PAUSED");

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
  
  // initialize the pop-up menu
  int paddle_color_menu = glutCreateMenu(paddleColorMenu);
  glutAddMenuEntry("Red",MENU_RED);
  glutAddMenuEntry("Black",MENU_BLACK);
  glutAddMenuEntry("Blue",MENU_BLUE);
  glutAddMenuEntry("Orange",MENU_ORANGE);
  glutAddMenuEntry("Cyan",MENU_CYAN);
  
  int ball_color_menu = glutCreateMenu(ballColorMenu);
  glutAddMenuEntry("Red",MENU_RED);
  glutAddMenuEntry("Black",MENU_BLACK);
  glutAddMenuEntry("Blue",MENU_BLUE);
  glutAddMenuEntry("Orange",MENU_ORANGE);
  glutAddMenuEntry("Cyan",MENU_CYAN);

  int menu = glutCreateMenu(popupMenu);
  glutAddMenuEntry("Restart Game",MENU_RESTART_GAME);
  glutAddMenuEntry("Toggle Pause",MENU_PAUSE);
  glutAddMenuEntry("Toggle AI (Resets Game)",MENU_TOGGLE_AI);
  glutAddSubMenu("Change Ball Color",ball_color_menu);
  glutAddSubMenu("Change Paddle Colors",paddle_color_menu);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutMainLoop();

  return 0;
}
