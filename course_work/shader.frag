#version 330 core
out vec4 FragColor;

in vec3 Pos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 FrontLights;
in vec3 BackLights;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};
uniform Material material;

void main()
{   
    vec4 color = texture(material.texture_diffuse1, TexCoords);
    if(color.a < 0.1)
        discard;
    //FragColor = color;
    
    //FragColor = vec4(FrontLights, 1);
    if(Normal.x > 0)
        FragColor = color * vec4(vec3(FrontLights.x), 1);
    else if(Normal.x < 0)
        FragColor = color * vec4(vec3(BackLights.x), 1);
    else if(Normal.y > 0)
        FragColor = color * vec4(vec3(FrontLights.y), 1);
    else if(Normal.y < 0)
        FragColor = color * vec4(vec3(BackLights.y), 1);
    else if(Normal.z > 0)
        FragColor = color * vec4(vec3(FrontLights.z), 1);
    else if(Normal.z < 0)
        FragColor = color * vec4(vec3(BackLights.z), 1);
    else 
        FragColor = color;
}  