#version 330 core
out vec4 FragColor;

uniform int uObjectID;

void main()
{
    FragColor = vec4(float(uObjectID)/255., 0, 0, 255);
}