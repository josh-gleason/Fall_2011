#include <cstdlib>
#include <iostream>
#include "PointTests.h"
#include "SketchPadDefs.h"

bool pointInTriangle(const vec4 input[3], const vec2& coord)
{
  vec2 A = vec2(input[0].x,input[0].y);
  vec2 B = vec2(input[1].x,input[1].y);
  vec2 C = vec2(input[2].x,input[2].y);
  
  vec2 v0 = C-A;
  vec2 v1 = B-A;
  vec2 v2 = coord-A;

  GLfloat dot00 = dot(v0, v0);
  GLfloat dot01 = dot(v0, v1);
  GLfloat dot02 = dot(v0, v2);
  GLfloat dot11 = dot(v1, v1);
  GLfloat dot12 = dot(v1, v2);

  GLfloat invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
  GLfloat u = (dot11 * dot02 - dot01 * dot12) * invDenom;
  GLfloat v = (dot00 * dot12 - dot01 * dot02) * invDenom;

  return (u>0) && (v>0) && (u+v<1);
}

bool pointNearLineSegment(const vec4 input[2], const vec2& coord, float thresh)
{
  // bounding box tests
  if ( coord.x-thresh > fmax(input[0].x,input[1].x) ) return false;
  if ( coord.x+thresh < fmin(input[0].x,input[1].x) ) return false;
  if ( coord.y-thresh > fmax(input[0].y,input[1].y) ) return false;
  if ( coord.y+thresh < fmin(input[0].y,input[1].y) ) return false;

  float dist;
  // use a constant time distance approximation (doesn't use square root)
  if ( !USE_DISTANCE_APPROX )
  {
    dist = fabs((input[1].x-input[0].x)*(input[0].y-coord.y   ) -
                (input[0].x-coord.x   )*(input[1].y-input[0].y)) /
           sqrt((input[1].x-input[0].x)*(input[1].x-input[0].x) +
                (input[1].y-input[0].y)*(input[1].y-input[0].y));
  }
  else
  {
    // using algorithm defined at ...
    // http://www.flipcode.com/archives/Fast_Approximate_Distance_Functions.shtml
    dist = fabs((input[1].x-input[0].x)*(input[0].y-coord.y   ) -
                (input[0].x-coord.x   )*(input[1].y-input[0].y));
    float x = fabs(input[1].x-input[0].x),
          y = fabs(input[1].y-input[0].y),
          max_p, min_p;
    
    if ( x > y )
    {
      max_p = x;
      min_p = y;
    }
    else
    {
      max_p = y;
      min_p = x;
    }
    
    dist /= 0.983398437*max_p+0.430664062*min_p-(max_p<16*min_p?0.0390625*max_p:0.0);
  }

  return ( dist < thresh );
}

bool pointNearPoint(const vec2 coord1, const vec2 coord2, float thresh)
{
  float dist;
  if ( !USE_DISTANCE_APPROX )
  {
    dist = sqrt((coord1.x-coord2.x)*(coord1.x-coord2.x) +
                (coord1.y-coord2.y)*(coord1.y-coord2.y));
  }
  else
  {
    float x = fabs(coord1.x-coord2.x),
          y = fabs(coord1.y-coord2.y),
          max_p, min_p;
    
    if ( x > y )
    {
      max_p = x;
      min_p = y;
    }
    else
    {
      max_p = y;
      min_p = x;
    }
    
    dist = 0.983398437*max_p+0.430664062*min_p-(max_p<16*min_p?0.0390625*max_p:0.0);
  }

  return ( dist < thresh );
}
