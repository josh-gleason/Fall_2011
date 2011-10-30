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
const int MODE_DRAW_POINT    = 6;
const int MODE_FREEFORM      = 7;

// should be generic to all shapes (except point)
const int MODE_SELECT          = 10;

const int MODE_ROTATE_SHAPE    = 11;
const int MODE_TRANSLATE_SHAPE = 12;
const int MODE_SCALE_SHAPE     = 13;

// minimum distance line needs to be from mouse to be "inside" (selected)
const float POINT_NEAR_LINE_THRESH = 0.02;

// min distance between adjacent vertices
const float MIN_VERTEX_SPACING     = 0.01;

const GLfloat MAX_THICKNESS        = 3.0;
const GLfloat MIN_THICKNESS        = 1.0;
const GLfloat THICKNESS_STEP       = 1.0;

// white and black
const vec4 COLOR_WHITE         = vec4(1.0,1.0,1.0,1.0);
const vec4 COLOR_BLACK         = vec4(0.0,0.0,0.0,1.0);

// primary colors
const vec4 COLOR_RED           = vec4(1.0,0.0,0.0,1.0);
const vec4 COLOR_GREEN         = vec4(0.0,1.0,0.0,1.0);
const vec4 COLOR_BLUE          = vec4(0.0,0.0,1.0,1.0);
const vec4 COLOR_CYAN          = vec4(0.0,1.0,1.0,1.0);
const vec4 COLOR_MAGENTA       = vec4(1.0,0.0,1.0,1.0);
const vec4 COLOR_YELLOW        = vec4(1.0,1.0,0.0,1.0);

// non-primary colors
const vec4 COLOR_ORANGE        = vec4(1.0,0.5,0.0,1.0);
const vec4 COLOR_PURPLE        = vec4(0.5,0.0,0.5,1.0);
const vec4 COLOR_VIOLET        = vec4(0.5,0.0,1.0,1.0);
const vec4 COLOR_BROWN         = vec4(0.6,0.3,0.0,1.0);

const int MENU_NEW     = 0;
const int MENU_SAVE    = 1;
const int MENU_SAVE_AS = 2;
const int MENU_OPEN    = 3;
const int MENU_RELOAD  = 4;
const int MENU_EXIT    = 5;

#endif // _JDG_SKETCHPAD_DEFS
