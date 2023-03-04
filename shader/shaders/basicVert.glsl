#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextCoord;
layout (location = 2) in float alight;
  
out vec2 textCoord;
out float light;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);
    textCoord = aTextCoord;
    light = alight;
} 