attribute vec4 vPosition;

void
main()
{
  gl_Position = gl_ModelViewProjectionMatrix * vPosition;
//  gl_Position = gl_ProjectionMatrix * vPosition;
}
