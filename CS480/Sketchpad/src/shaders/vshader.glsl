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
  
  // construct final model-view matrix
  mat4 modelview = t4 * t3 * t2 * t1;

  // compute final vertex position
  gl_Position = projection * modelview * vPosition;
}
