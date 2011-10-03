#include "Ball.h"

Ball::Ball(const vec2& center, GLfloat radius, const vec4& color,
  const vec2& velocity, GLfloat bottomWall, GLfloat topWall, GLfloat leftGoal,
  GLfloat rightGoal, GLfloat xAccel, GLfloat yAccel, GLfloat maxSpeed)
: pause_count(0),
  _circle(center, radius, color),
  _start_center(center),
  _radius(radius),
  _max_y(topWall),
  _min_y(bottomWall),
  _max_x(rightGoal),
  _min_x(leftGoal),
  _velocity(velocity),
  _x_accel(xAccel),
  _y_accel(yAccel),
  _max_speed(maxSpeed)
{}

Ball::Ball( const Ball& rhs )
: 
  _circle(rhs._circle),
  _projection(rhs._projection),
  _translation(rhs._translation),
  _start_center(rhs._start_center),
  _radius(rhs._radius),
  _max_y(rhs._max_y),
  _min_y(rhs._min_y),
  _max_x(rhs._max_x),
  _min_x(rhs._min_x),
  _velocity(rhs._velocity),
  _x_accel(rhs._x_accel),
  _y_accel(rhs._y_accel),
  _max_speed(rhs._max_speed)
{}

const Ball& Ball::operator= ( const Ball& rhs )
{
  _circle = rhs._circle;
  _projection = rhs._projection;
  _translation = rhs._translation;
  _start_center = rhs._start_center;
  _radius = rhs._radius;
  _max_y = rhs._max_y;
  _min_y = rhs._min_y;
  _max_x = rhs._max_x;
  _min_x = rhs._min_x;
  _velocity = rhs._velocity;
  _x_accel = rhs._x_accel;
  _y_accel = rhs._y_accel;
  _max_speed = rhs._max_speed;
  return *this;
}

GLfloat Ball::getLeftX() const
{
  return _start_center[0] - _radius + _translation[0];
}

GLfloat Ball::getRightX() const
{
  return _start_center[0] + _radius + _translation[0];
}

GLfloat Ball::getTopY() const
{
  return _start_center[1] + _radius + _translation[1];
}

GLfloat Ball::getBottomY() const
{
  return _start_center[1] - _radius + _translation[1];
}

GLfloat Ball::getCenterX() const
{
  return _start_center[0] + _translation[0];
}

GLfloat Ball::getCenterY() const
{
  return _start_center[1] + _translation[1];
}

vec2 Ball::getCenter() const
{
  return _start_center + _translation;
}

vec2 Ball::getUpperLeft() const
{
  return vec2(getLeftX(),getTopY());
}

vec2 Ball::getLowerRight() const
{
  return vec2(getRightX(),getBottomY());
}

GLfloat Ball::getDx() const
{
  return _velocity[0];
}

GLfloat Ball::getDy() const
{
  return _velocity[1];
}

vec2 Ball::getVelocity() const
{
  return _velocity;
}

void Ball::reset(const vec2& center, const vec2& velocity, int pauseCount)
{
  _translation = center - _start_center;
  _velocity = velocity;
  pause_count = pauseCount;
}

void Ball::setBounds(GLfloat bottomWall, GLfloat topWall, GLfloat leftGoal,
  GLfloat rightGoal)
{
  _max_y = topWall;
  _min_y = bottomWall;
  _max_x = rightGoal;
  _min_x = leftGoal;
}

void Ball::setProjection(const mat4& projection )
{
  _projection = projection;
}

void Ball::setProjection(GLfloat left, GLfloat right, GLfloat top,
  GLfloat bottom)
{
  _projection = Ortho2D(left, right, top, bottom);
}

void Ball::setColor(const vec4& color)
{
  _circle.setColor(color);
}

void Ball::init()
{
  _circle.init();
}

void Ball::draw() const
{
  vec4 transl;
  transl[0] = _translation[0];
  transl[1] = _translation[1];
  transl[2] = transl[3] = 0;
  _circle.draw(_projection, transl); 
}

int Ball::takeStep(const Paddle& left_paddle, const Paddle& right_paddle)
{
  if ( pause_count > 0 )
  {
    pause_count--;
    return NO_GOAL;
  }

  _translation += _velocity;

  // check top and bottom
  if ( ( getTopY() > _max_y && _velocity[1] > 0    ) ||
       ( getBottomY() < _min_y && _velocity[1] < 0 ) )
  {
    _velocity[1] *= -1;
    _translation[1] += 2*_velocity[1];
  }

  if ( _velocity[0] < 0 && // check left paddle
       getLeftX() <= left_paddle.getRightX() &&
       getBottomY() <= left_paddle.getTopY()-_radius*0.5 &&
       getTopY() >= left_paddle.getBottomY()+_radius*0.5 )
  {
    if ( getLeftX() - _velocity[0] > left_paddle.getRightX() )
    {
      // check if paddle is moving
      if ( left_paddle.getLastMove() == Paddle::UP
           && abs(_velocity[1]) < _max_speed )
        _velocity[1] += _y_accel;
      else if ( left_paddle.getLastMove() == Paddle::DOWN
                && abs(_velocity[1]) < _max_speed )
        _velocity[1] -= _y_accel;

      _velocity[0] *= -1;
      _translation[0] += 2*_velocity[0];
      
      if ( abs(_velocity[0]) < _max_speed )
        _velocity[0] += (_velocity[0] > 0 ? _x_accel : -_x_accel);
    }
    else  // already lost but ball should reflect off top/bot of paddle
    {
      // determine which direction ball should go by which end of paddle is
      // closer
      if ( abs(getCenterY() - left_paddle.getBottomY()) <
           abs(getCenterY() - left_paddle.getTopY()) )
      {
        _velocity[1] = -1*abs(_velocity[1]);  // force down
        _translation[1] = left_paddle.getBottomY() - _radius
                          - _start_center[1];
      }
      else
      {
        _velocity[1] = abs(_velocity[1]);     // force up
        _translation[1] = left_paddle.getTopY() + _radius 
                          - _start_center[1];
      }
    }
  }
  else if ( _velocity[0] > 0 && // check right paddle
       getRightX() >= right_paddle.getLeftX() &&
       getBottomY() <= right_paddle.getTopY()-_radius*0.5 &&
       getTopY() >= right_paddle.getBottomY()+_radius*0.5 )
  {  
    if ( getRightX() - _velocity[0] < right_paddle.getLeftX() )
    {
      // check if paddle is moving
      if ( right_paddle.getLastMove() == Paddle::UP 
        && abs(_velocity[1]) < _max_speed )
        _velocity[1] += _y_accel;
      else if ( right_paddle.getLastMove() == Paddle::DOWN
        && abs(_velocity[1]) < _max_speed )
        _velocity[1] -= _y_accel;
      
      _velocity[0] *= -1;
      _translation[0] += 2*_velocity[0];
      
      // speed up the ball every hit
      if ( abs(_velocity[0]) < _max_speed )
        _velocity[0] += (_velocity[0] > 0 ? _x_accel : -_x_accel);
    }
    else
    {
      // determine which direction ball should go by which end of paddle is
      // closer
      if ( abs(getCenterY() - right_paddle.getBottomY()) <
           abs(getCenterY() - right_paddle.getTopY()) )
      {
        _velocity[1] = -1*abs(_velocity[1]);  // force down
        
        // directly set location to place ball on bottom of paddle
        _translation[1] = right_paddle.getBottomY() - _radius
                          - _start_center[1];
      }
      else
      {
        _velocity[1] = abs(_velocity[1]);     // force up
        _translation[1] = right_paddle.getTopY() + _radius 
                          - _start_center[1];
      }
    }

  }
  
  if ( getRightX() < _min_x ) // left side lost
    return SCORE_RIGHT;
  if ( getLeftX() > _max_x )  // right side lost
    return SCORE_LEFT; 
    
  return 0;
}

