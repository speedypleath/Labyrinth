#version 410

layout (location = 0) in vec4 in_Position; // pozitia este atribut standard
layout (location = 1) in vec4 in_Color; // culoarea este atribut instantiat
layout (location = 2) in mat4 modelMatrix; // matricea de transformare este atribut instantiat

out vec4 gl_Position; 
out vec4 ex_Color;
 
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
 
void main(void)
{
    gl_Position = projectionMatrix 
    * viewMatrix 
    * modelMatrix 
    * rotate(mat4(1.0f), 3.14f / 2.0f, vec3(0.0, 1.0, 0.0)) 
    * translate(mat4(1.0f), vec3(-100.0, 0.0, 30.0)) 
    * in_Position;
	ex_Color=in_Color;
} 
