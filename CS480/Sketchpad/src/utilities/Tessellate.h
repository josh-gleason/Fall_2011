#ifndef _JDG_UTILITIES_TESSELLATE_
#define _JDG_UTILITIES_TESSELLATE_

#include <vector>
#include "vec.h"

// tessellate a polygon into triangles (assumes the vertices of a line
// loop are passed as input and vertices of a set of triangles passed back
// as output)
void tessellate(const std::vector<vec2>& input, vec4** output, int& outsize);

#endif // _JDG_UTILITIES_TESSELATE_
