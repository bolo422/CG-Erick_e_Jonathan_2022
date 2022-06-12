#version 460 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
    vec3 position;  
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;

uniform Light light;
uniform Light sun;

uniform bool selected;

void main()
{    
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
    
    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    
    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse   *= attenuation;
    specular *= attenuation;   
        
    vec3 result = ambient + diffuse + specular;
    
    /////////////////////////////

    // ambient
    ambient = sun.ambient * texture(material.diffuse, TexCoords).rgb;
    
    // diffuse 
    norm = normalize(Normal);
    lightDir = normalize(sun.position - FragPos);
    diff = max(dot(norm, lightDir), 0.0);
    diffuse = sun.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular
    viewDir = normalize(viewPos - FragPos);
    reflectDir = reflect(-lightDir, norm);  
    spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    specular = sun.specular * spec * texture(material.specular, TexCoords).rgb;  
    
    // spotlight (soft edges)
    theta = dot(lightDir, normalize(-sun.direction)); 
    epsilon = (sun.cutOff - sun.outerCutOff);
    intensity = clamp((theta - sun.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    
    // attenuation
    distance    = length(sun.position - FragPos);
    attenuation = 1.0 / (sun.constant + sun.linear * distance + sun.quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse   *= attenuation;
    specular *= attenuation;   
        
    vec3 result2 = ambient + diffuse + specular;



    ////////////////////////
    if(selected)
    {
        FragColor = vec4(result + result2, 1.0) * vec4(0.5,1.0,1.0, 1.0);
    }
    else
    {
        FragColor = vec4(result + result2, 1.0);
    }
    
}