#include "Paddle.h"

Paddle::Paddle(const vec2& upLeft, const vec2& lowRight, const vec4& color,
  GLfloat stepSize, GLfloat bottomWall, GLfloat topWall)
: move_flag(STILL),
  last_move(STILL),
  rect(upLeft, lowRight, color),
  projection(),   // creates identity projection
  translation(),  // no translation yet
  step_size(stepSize),
  max_y(topWall),
  min_y(bottomWall),
  start_x(upLeft.x),
  start_y(upLeft.y),
  start_x2(lowRight.x),
  start_y2(lowRight.y)
{}

Paddle::Paddle( const Paddle& rhs )
: move_flag(rhs.move_flag),
  last_move(rhs.last_move),
  rect(rhs.rect),
  projection(rhs.projection),
  translation(rhs.translation),
  step_size(rhs.step_size),
  max_y(rhs.max_y),
  min_y(rhs.min_y),
  start_x(rhs.start_x),
  start_y(rhs.start_y),
  start_x2(rhs.start_x2),
  start_y2(rhs.start_y2)
{}

const Paddle& Paddle::operator= ( const Paddle& rhs )
{
  move_flag = rhs.move_flag;
  last_move = rhs.last_move;
  rect = rhs.rect;
  projection = rhs.projection;
  translation = rhs.translation;
  step_size = rhs.step_size;
  max_y = rhs.max_y;
  min_y = rhs.min_y;
  start_x = rhs.start_x;
  start_y = rhs.start_y;
  start_x2 = rhs.start_x2;
  start_y2 = rhs.start_y2;
  return *this;
}

GLfloat Paddle::getLastMove() const
{
  return last_move;
}

GLfloat Paddle::getLeftX() const
{
  return start_x+translation[0];
}

GLfloat Paddle::getRightX() const
{
  return start_x2+translation[0];
}

GLfloat Paddle::getTopY() const
{
  return start_y+translation[1];
}

GLfloat Paddle::getBottomY() const
{
  return start_y2+translation[1];
}

GLfloat Paddle::getCenterX() const
{
  return (start_x+start_x2)/2.0+translation[0];
}

GLfloat Paddle::getCenterY() const
{
  return (start_y+start_y2)/2.0+translation[1];
}

vec2 Paddle::getCenter() const
{
  return vec2(getCenterX(),getCenterY());
}

vec2 Paddle::getUpperLeft() const
{
  return vec2(getLeftX(),getTopY());
}

vec2 Paddle::getLowerRight() const
{
  return vec2(getRightX(),getBottomY());
}

void Paddle::setBounds(GLfloat bottomWall, GLfloat topWall)
{
  min_y = bottomWall;
  max_y = topWall;
}

void Paddle::setTranslation( GLfloat xTrans, GLfloat yTrans )
{
  translation[0] = xTrans;
  translation[1] = yTrans;
  translation[2] = translation[3] = 0;
}

void Paddle::setTranslation( const vec4& translationVec )
{
  translation = translationVec;
}

void Paddle::setProjection(const mat4& projectionMat)
{
  projection = projectionMat;
}

void Paddle::setProjection(GLfloat left, GLfloat right, GLfloat top,
  GLfloat bottom)
{
  projection = Ortho2D(left, right, top, bottom);
}

void Paddle::setColor( const vec4& newColor )
{
  rect.setColor(newColor);
}

void Paddle::init()
{
  rect.init();
}

void Paddle::draw() const
{
  rect.draw(projection,translation);
}

GLfloat Paddle::getStepSize() const
{
  return step_size;
}

void Paddle::setStepSize(GLfloat stepSize)
{
  if ( stepSize > 0 )
    step_size = stepSize;
}

void Paddle::takeStep()
{
  switch (move_flag)
  {
    case DOWN:
      if ( start_y2+translation[1] - step_size < min_y )
        last_move = STILL;
      else
      {
        translation[1] -= step_size;
        last_move = DOWN;
      }
      break;
    case UP:
      if ( start_y+translation[1] + step_size > max_y )
        last_move = STILL;
      else
      {
        translation[1] += step_size;
        last_move = UP;
      }
      break;
    default:   // STILL
      last_move = STILL;
  }
}
