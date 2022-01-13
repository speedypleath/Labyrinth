#version 410

layout (location = 0) in vec4 in_Position;
layout (location = 1) in vec4 in_Color;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in mat4 modelMatrix;

out vec4 gl_Position; 
out vec4 ex_Color;
out vec2 tex_Coord;
 
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
 
void main(void)
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * in_Position;
    tex_Coord = vec2(texCoord.x, texCoord.y);
	ex_Color=in_Color;
} 
