#version 410

in vec4 ex_Color;
in vec2 tex_Coord;
out vec4 out_Color;

uniform int codCol;
uniform sampler2D tex_Unit;

void main(void)
{
  if(codCol == 0) {
    out_Color = ex_Color;
  }
  else {
    out_Color = texture(tex_Unit, tex_Coord);
  }
}