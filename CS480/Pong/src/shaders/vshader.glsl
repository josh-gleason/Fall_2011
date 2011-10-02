uniform mat4 projection;
uniform vec4 translation;

attribute vec4 vPosition;
attribute vec4 vColor;
varying vec4 out_color;

void
main()
{
    out_color = vColor;
    gl_Position = projection*(vPosition+translation);
}
