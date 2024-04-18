#shader vertex
#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos;
out vec3 ourNormal; // output a normal to the fragment shader

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0f);
	fragPos = vec3(model * vec4(pos, 1.0));
	ourNormal = normal;
}

#shader fragment
#version 330 core
struct Material {
	vec3 ambient; //Jaki kolor sie odbija pod wplywem ambient light - zazwyczaj to kolor po prostu obiektu 
	vec3 diffuse;
	vec3 specular;
	float shineness;
};
uniform Material material;

in vec3 ourNormal;
in vec3 fragPos;
out vec4 fragColor;

uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
	//Diffuse
	vec3 norm = normalize(ourNormal);
	vec3 lightDirection = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDirection), 0.0f);
	vec3 diffuse = (diff * material.diffuse) * lightColor;

	//Ambient
	vec3 ambient = lightColor * material.ambient;

	//Specular
	vec3 viewDirection = normalize(viewPos - fragPos);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shineness * 128);
	vec3 specular = lightColor * (spec * material.specular);

	vec3 result = ambient + diffuse + specular;
;
    fragColor = vec4(result, 1.0);
}