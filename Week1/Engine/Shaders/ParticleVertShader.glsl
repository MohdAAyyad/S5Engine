#version 460 core

uniform vec3 position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform float size;
uniform vec4 color;

out vec4 fcolor;

void main()
{
vec4 dist = view * model * vec4(position,1.0f);
float atten = inversesqrt(0.1f) * length(dist.xyz);
gl_PointSize = atten * size;
gl_Position = proj * view * model * vec4(position,1.0f);

fcolor = color;
}