attribute vec4 vPosition;

// 2d center of object
uniform vec2 center;

// translation, rotation and scaling parameters
uniform vec2  translate;
uniform float theta;
uniform vec2  scale;

// projection matrix
uniform mat4  projection;

void main()
{
  float thetaRad = theta*0.017453292519943; // theta*PI/180
  float s = sin(thetaRad);
  float c = cos(thetaRad);

  // Note: Matricies look like they are transposed because GLSL is column major

  // all scaling and rotation is done around the center
  mat4 t1 = mat4(1.0,       0.0,       0.0, 0.0,
                 0.0,       1.0,       0.0, 0.0,
                 0.0,       0.0,       1.0, 0.0,
                 -center.x, -center.y, 0.0, 1.0);

  // rotate
  mat4 t2 = mat4(c,   s,   0.0, 0.0,
                 -s,  c,   0.0, 0.0,
                 0.0, 0.0, 1.0, 0.0,
                 0.0, 0.0, 0.0, 1.0);

  // scale
  mat4 t3 = mat4(scale.x, 0.0,     0.0, 0.0,
                 0.0,     scale.y, 0.0, 0.0,
                 0.0,     0.0,     1.0, 0.0,
                 0.0,     0.0,     0.0, 1.0);

  // translate (and translate back to center)
  mat4 t4 = mat4(1.0,                  0.0,                  0.0, 0.0,
                 0.0,                  1.0,                  0.0, 0.0,
                 0.0,                  0.0,                  1.0, 0.0,
                 center.x+translate.x, center.y+translate.y, 0.0, 1.0);

  // construct current transformation matrix
  mat4 ctm = projection * t4 * t3 * t2 * t1;

  // compute final vertex position
  gl_Position = ctm * vPosition;
}

/* This doesn't seem to make much difference in speed when testing so I'm
   leaving the code more understandable
// this is the product t4*t3*t2*t1 determined symbolically
mat4 modelview = mat4(c*scale.x, s*scale.y, 0.0, 0.0,
                      -s*scale.x, c*scale.y, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0,
                      center.x*(1.0-c*scale.x)+center.y*scale.x*s+translate.x,
                      center.y*(1.0-c*scale.y)-center.x*scale.y*s+translate.y,
                      0.0, 1.0); */

