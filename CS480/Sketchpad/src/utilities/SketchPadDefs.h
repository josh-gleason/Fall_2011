#ifndef _JDG_SKETCHPAD_DEFS_
#define _JDG_SKETCHPAD_DEFS_

// use a distance approximation rather than actual distance formula (for speed)
const bool USE_DISTANCE_APPROX = true;

// mouse constants
const int DRAW_RECT   = 0;
const int DRAW_LINE   = 1;
const int DRAW_CIRCLE = 2;

// should be generic to all shapes (except point)
const int ROTATE_SHAPE    = 3;
const int TRANSLATE_SHAPE = 4;
const int SCALE_SHAPE     = 5;

// minimum distance line needs to be from mouse to be "inside" (selected)
const float POINT_NEAR_LINE_THRESH = 0.02;

#endif // _JDG_SKETCHPAD_DEFS
