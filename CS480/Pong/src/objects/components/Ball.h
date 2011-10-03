/** @file Ball.h
    @brief Contains all pieces of a ball object. */

#ifndef __JDG_BALL__
#define __JDG_BALL__

#include "Circle.h"
#include "Paddle.h"

/** @brief Ball object holds graphics and properties of the ball */
class Ball
{
public:
  static const int NO_GOAL = 0;
  static const int SCORE_LEFT = 1;
  static const int SCORE_RIGHT = 2;

  /** @brief initialize a new ball */
  Ball(const vec2& center   = vec2(0.0,0.0),
       GLfloat radius       = 0.04,
       const vec4& color    = vec4(0.8,0.2,0.2,1.0), 
       const vec2& velocity = vec2(0.01,0.01),
       GLfloat bottomWall   = -1.0,
       GLfloat topWall      = 1.0,
       GLfloat leftGoal     = -1.0,
       GLfloat rightGoal    = 1.0,
       GLfloat xAccel       = 0.0003,
       GLfloat yAccel       = 0.0022,
       GLfloat maxSpeed     = 0.05);

  Ball( const Ball& rhs );
  const Ball& operator= ( const Ball& rhs );

  GLfloat getLeftX() const;   ///< @brief get left edge location
  GLfloat getRightX() const;  ///< @brief get right edge location
  GLfloat getTopY() const;    ///< @brief get top edge location
  GLfloat getBottomY() const; ///< @brief get bottom edge location
  GLfloat getCenterX() const; ///< @brief get center x coordinate
  GLfloat getCenterY() const; ///< @brief get center y coordinate
  vec2 getCenter() const;     ///< @brief get center location of ball
  vec2 getUpperLeft() const;  ///< @brief get upper left point
  vec2 getLowerRight() const; ///< @brief get lower right point

  GLfloat getDx() const;      ///< @brief get the x velocity
  GLfloat getDy() const;      ///< @brief get the y velocity
  vec2 getVelocity() const;   ///< @brief get the velocity vector

  /** @brief reset the balls position and velocity */
  void reset(const vec2& center   = vec2(0.0,0.0),
             const vec2& velocity = vec2(0.01,0.01),
             int pauseCount       = 0);
  
  /** @brief set the location of the lower and upper bound when moving */
  void setBounds(GLfloat bottomWall,
                 GLfloat topWall,
                 GLfloat leftGoal,
                 GLfloat rightGoal);

  /** @brief set the projection matrix directly. */
  void setProjection(const mat4& projection);
  /** @brief set the projection matrix using the 2D orthographic projection
    *        equations. */
  void setProjection(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom);

  /** @brief set the color of all the vertices **/
  void setColor(const vec4& colorVec);

  /** @brief initialize the shaders */
  void init();

  /** @brief draw the paddle */
  void draw() const;

  /** @brief move the ball (if pause_count <= 0), collide with paddles
             and walls correctly. Usually returns SUCCESS, but if a point
             is scored either LEFT or RIGHT is returned which represents
             which player scored. (i.e, if the left paddle misses a ball
             RIGHT would be returned) */
  int takeStep(const Paddle& left, const Paddle& right);

  /** @brief If this value is greater than zero, then the ball will not move
             when takeStep() is called.  This is used for pausing the ball for
             a set number of frames right before a round starts. */
  int pause_count;
protected:
  // object which displays the ball
  Circle _circle;
  mat4 _projection;
  vec2 _translation;

  // current center of ball and radius
  vec2 _start_center;
  GLfloat _radius;

  // ball should reflect off of the ceiling here
  GLfloat _max_y;
  GLfloat _min_y;
  GLfloat _max_x;
  GLfloat _min_x;

  // velocity vector (distance/frame)
  vec2 _velocity;

  // speed thresholds and parameters
  GLfloat _x_accel; // acceleration due to hitting paddles
  GLfloat _y_accel; // acceleration due to hitting moving paddle

  GLfloat _max_speed; // max speed in any direction
};

#endif // __JDG_BALL__
