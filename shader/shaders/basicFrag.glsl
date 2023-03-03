#version 330 core
out vec4 FragColor;  

in vec2 textCoord;
in float light;

uniform sampler2D ourTexture;
  
void main()
{
    FragColor = texture(ourTexture, textCoord) * light;
}