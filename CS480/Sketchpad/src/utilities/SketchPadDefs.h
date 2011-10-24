#ifndef _JDG_SKETCHPAD_DEFS_
#define _JDG_SKETCHPAD_DEFS_

// use a distance approximation rather than actual distance formula (for speed)
const bool USE_DISTANCE_APPROX = true;

// mouse constants
const int MODE_NONE          = 0;
const int MODE_DRAW_RECT     = 1;
const int MODE_DRAW_LINE     = 2;
const int MODE_DRAW_POLY     = 3;
const int MODE_DRAW_LINE_SEG = 4;
const int MODE_DRAW_CIRCLE   = 5;

// should be generic to all shapes (except point)
const int MODE_SELECT          = 10;

const int MODE_ROTATE_SHAPE    = 11;
const int MODE_TRANSLATE_SHAPE = 12;
const int MODE_SCALE_SHAPE     = 13;
const int MODE_FILL_SHAPE      = 14;
const int MODE_CHANGE_COLOR    = 15;

// minimum distance line needs to be from mouse to be "inside" (selected)
const float POINT_NEAR_LINE_THRESH = 0.02;

#endif // _JDG_SKETCHPAD_DEFS
