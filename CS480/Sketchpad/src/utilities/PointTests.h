#ifndef _JDG_POINT_TESTS_
#define _JDG_POINT_TESTS_
  #include "vec.h"
  // returns true of coord falls inside the triangle defined by 3 input vectors
  // only first 2 values of the vec4 are used. Barycentric coordinates are used
  // in this function
  bool pointInTriangle(const vec4 input[3], const vec2& coord);

  // checks if a point is within a certain distance of a line
  bool pointNearLineSegment(const vec4 input[2], const vec2& coord, float thresh);

  // checks if a point is within a certain distance of another point
  bool pointNearPoint(const vec2 coord1, const vec2 coord2, float thresh);
#endif
