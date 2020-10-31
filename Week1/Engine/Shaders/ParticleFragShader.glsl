#version 460 core

in vec4 fcolor;
out vec4 color;
uniform sampler2D inputTexture;

void main()
{
vec4 tmpColor =  texture(inputTexture,  gl_PointCoord) * fcolor;
color = tmpColor;
}