attribute vec4 vPosition;
attribute vec4 vColor;
varying vec4 out_color;

void
main()
{
    gl_Position = vPosition;
    out_color = vColor;
}
