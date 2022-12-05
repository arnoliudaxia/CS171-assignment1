#version 330 core

// Interpolated values from the vertex shaders
in vec3 fragmentColor;

in vec3 normalV;
in vec3 FragPos;

uniform vec3 ambientLightColor;
uniform float ambientLightStrength;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float specularStrength;
uniform vec3 OBJOcolor;
// Ouput data
out vec3 color;

void main(){
	vec3 objColor=fragmentColor;
	if(objColor==vec3(0,0,0))
	{
	objColor=OBJOcolor;
	}
	vec3 ambient = ambientLightStrength * ambientLightColor;
	//Diffuse
	vec3 norm = normalize(normalV);
	vec3 lightDir = normalize(lightPos - FragPos);  
	float diff = max((dot(norm, lightDir)), 0.0);
	vec3 diffuse = diff * ambientLightColor;
	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
vec3 specular = specularStrength * spec * ambientLightColor;  
	
	color=(ambient+diffuse+ specular)*objColor;
	
}