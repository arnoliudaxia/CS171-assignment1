#version 330 core

// Interpolated values from the vertex shaders
in vec3 fragmentColor;

in vec3 normalV;
in vec3 FragPos;

//Direct light
struct DirLight {
    vec3 direction;
    vec3 LightColor;
    float ambientLightStrength;
    float specularStrength;
    int isEnable;
};  
uniform DirLight dirLight;
struct PointLight {
    vec3 lightPosition;
    vec3 LightColor;
    float ambientLightStrength;
    float specularStrength;
    int isEnable;
};  
uniform PointLight pointLight;
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 LightColor;
    float ambientLightStrength;
    float specularStrength;

	float lightPower;
    int isEnable;      
};
uniform SpotLight spotLight;

uniform vec3 viewPos;
uniform vec3 OBJOcolor;
// Ouput data
out vec3 color;

vec3 CalcDirLight(DirLight light, vec3 viewDir)
{
    if(light.isEnable==0)
    {
    return vec3(0);
    }
    vec3 lightDir = normalize(-light.direction);

    // ambient
    vec3 ambient  = light.ambientLightStrength * light.LightColor;
    //Diffuse
	vec3 norm = normalize(normalV);
	float diff = max((dot(norm, lightDir)), 0.0);
	vec3 diffuse = diff * light.LightColor;
	//specular
	vec3 reflectDir = reflect(-lightDir, normalV);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specularStrength * spec * light.LightColor ; 
    
    return (ambient + diffuse + specular);
}  
vec3 CalcPointLight(PointLight light, vec3 viewDir)
{
    if(light.isEnable==0)
    {
    return vec3(0);
    }
    // ambient
        vec3 ambient  = light.ambientLightStrength * light.LightColor;
	    //Diffuse
	    vec3 norm = normalize(normalV);
	    vec3 lightDir = normalize(light.lightPosition - FragPos);  
	    float diff = max((dot(norm, lightDir)), 0.0);
	    vec3 diffuse = diff * light.LightColor;
	    //specular
	    vec3 reflectDir = reflect(-lightDir, norm);  
	    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specularStrength * spec * light.LightColor;  
    return (ambient + diffuse + specular);
	
}

vec3 CalcSpotLight(SpotLight light, vec3 viewDir)
{
if(light.isEnable==0)
{
return vec3(0);
}
    vec3 lightDir = normalize(light.position - FragPos);
    // diffuse shading
    float diff = max(dot(normalV, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normalV);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float edge=length(lightDir)*light.cutOff; 
    float lightN = dot(lightDir, normalize(-light.direction)); 
    float intensity = 0;
	if(lightN>edge)
    {
	intensity=1;
    }
    // combine results
    vec3 ambient  = light.ambientLightStrength * light.LightColor;
    vec3 diffuse = diff * light.LightColor;
    vec3 specular = light.specularStrength * spec * light.LightColor;  
    return (ambient + diffuse + specular)*light.lightPower* attenuation * intensity;
}

void main(){

	vec3 objColor=fragmentColor;
	if(objColor==vec3(0,0,0))
	{
	objColor=OBJOcolor;
	}
	if(pointLight.isEnable+dirLight.isEnable+spotLight.isEnable==0)
{
color=objColor;
}
else{
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 ouputc=vec3(0);
	ouputc+=objColor*CalcPointLight(pointLight,viewDir);
		ouputc+=objColor*CalcDirLight(dirLight,viewDir);
	ouputc+=objColor*CalcSpotLight(spotLight,viewDir);
	color=ouputc;
	}
}