#version 330 core

out vec4 FragColor;  

struct Light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
};


in vec3 FragPos;  
in vec3 Normal;  
in vec2 textCoord;

uniform sampler2D ourTexture;
uniform vec3 viewPos;
uniform Light light;
  
void main()
{
    // ambient
    vec3 ambient = light.ambient;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff;

    vec3 result = ambient + diffuse;
    FragColor = (texture(ourTexture, textCoord) * vec4(result, 1.0));
}