#version 450 core

out vec4 FragColor;  

struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 color;
};

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D ourTexture;
uniform sampler2D shadowMap;
uniform vec3 viewPos;
uniform Light light;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow

    float bias = max(0.02 * (1.0 - dot(fs_in.Normal, light.direction)), 0.002);
    float shadow = (currentDepth - bias) > closestDepth  ? 1.0 : 0.0;

    return shadow;
}
  
void main()
{
    // ambient
    vec3 ambient = light.ambient * light.color;

    // diffuse 
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(-light.direction);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff;
    diffuse = diffuse * light.color;

//    vec3 result = ambient + diffuse;
//    FragColor = (texture(ourTexture, fs_in.TexCoords) * vec4(result, 1.0));

    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);                      
    vec3 lighting = (ambient + (1-shadow) * (diffuse));
    FragColor = (texture(ourTexture, fs_in.TexCoords)) * vec4(lighting, 1.0);
}