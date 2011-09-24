/** @file Paddle.cc
  * @brief Contains all pieces of a paddle object. */

#ifndef __JDG_PADDLE__
#define __JDG_PADDLE__

#include "Rect.h"

/** @brief Paddle object used to hold the graphics and the properties of
  *        the paddle. */
class Paddle
{
public:
  enum MvDir {UP, DOWN, STILL};
  
  /** @brief initialize a new paddle */
  Paddle(const vec2& upLeft   = vec2(-0.1,0.1),
         const vec2& lowRight = vec2(-0.1,0.1),
         const vec4& color    = vec4(0.8,0.2,0.2,1.0),
         GLfloat stepSize     = 0.02,
         GLfloat bottomWall   = -1.0,
         GLfloat topWall      = 1.0);

  Paddle( const Paddle& rhs );
  const Paddle& operator= ( const Paddle& rhs );

  GLfloat leftX() const;   ///< @brief get left edge location
  GLfloat rightX() const;  ///< @brief get right edge location
  GLfloat topY() const;    ///< @brief get top edge location
  GLfloat bottomY() const; ///< @brief get bottom edge location
  vec2 upperLeft() const;  ///< @brief get upper left point
  vec2 lowerRight() const; ///< @brief get lower right point
  
  /** @brief set the location of the lower and upper bound when moving */
  void setBounds(GLfloat bottom_wall, GLfloat top_wall);

  /** @brief set the projection matrix directly. */
  void setProjection(const mat4& projection);
  /** @brief set the projection matrix using the 2D orthographic projection
    *        equations. */
  void setProjection(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom);

  /** @brief draw the paddle */
  void draw() const;

  // Returns the step size
  GLfloat getStep() const;
  void setStep();

  /** @brief returns false if at a step cannot be taken (hit edge), if
    *        this returns false, move_flag is set to STILL */
  bool takeStep();
  
  /** @brief can be UP, DOWN or STILL */
  MvDir move_flag;
protected:
  Rect rect;
  mat4 projection;
  mat4 translation;

  /** @brief step size when moving */
  GLfloat step_size;

  /** @brief defines the paddle boundries */
  GLfloat max_x, min_x;

  /** @brief starting point of the paddle */
  GLfloat start_x, start_y, start_x2, start_y2;
};

#endif // __JDG_PADDLE__

