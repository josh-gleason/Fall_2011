#include "Paddle.h"

Paddle::Paddle(const vec2& upLeft, const vec2& lowRight, const vec4& color,
  GLfloat stepSize, GLfloat bottomWall, GLfloat topWall)
: move_flag(STILL),
  rect(upLeft, lowRight, color),
  projection(),   // creates identity projection
  translation(),  // no translation yet
  step_size(stepSize),
  max_x(topWall),
  min_x(bottomWall),
  start_x(upLeft.x),
  start_y(upLeft.y),
  start_x2(lowRight.x),
  start_y2(lowRight.y)
{}
