#version 330 core
out vec4 FragColor;
  
in vec2 vTexCoord;
flat in int vDiscard;

uniform vec4 uColor;

void main()
{
    if (vDiscard > 0) discard;
    // map to circle
    vec2 circle_p = vTexCoord - 0.5;
    const float radius = 0.5;
    if (circle_p.x*circle_p.x + circle_p.y*circle_p.y > (radius*radius))
        discard;
    FragColor = uColor;
}