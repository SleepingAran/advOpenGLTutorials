#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec3 FragPos;  
in vec3 Normal;  
  
out vec4 color;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light[4];
void main()
{
    vec3 result;
    for (int i = 0; i < 4; i++)
    {
        // Ambient
        vec3 ambient = light[i].ambient * material.ambient;
  	
        // Diffuse 
        vec3 norm = normalize(Normal);
	    vec3 lightDir = normalize(light[i].position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light[i].diffuse * (diff * material.diffuse);
    
        // Specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light[i].specular * (spec * material.specular);  
    
	    //Attenuation
	    
	    float distance = length(light[i].position - FragPos);
	    float attenuation = 1.0f / (light[i].constant + light[i].linear * distance + 
    	light[i].quadratic * (distance * distance));
        ambient *= attenuation;
	    diffuse *= attenuation;
    	specular *= attenuation;

	    result += ambient;
        result += diffuse;
        result += specular;
        color = vec4(result, 1.0f);
    }
} 