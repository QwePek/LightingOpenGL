#shader vertex
#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoord;

out vec2 texCoord;

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
	texCoord = textureCoord;
}

#shader fragment
#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shineness;
};
uniform Material material0;

struct DirectionalLight {
	vec3 direction;

	vec3 ambient; //Teraz kontrolujemy ambient Materialu poprzez ambient Light
	vec3 diffuse;
	vec3 specular;
};

#define NR_OF_POINT_LIGHTS 1
struct PointLight {
	vec3 position;

	vec3 ambient; //Teraz kontrolujemy ambient Materialu poprzez ambient Light
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform DirectionalLight dirLight;
uniform PointLight pointLight[NR_OF_POINT_LIGHTS];
uniform vec3 viewPos;

in vec3 fragPos;
in vec3 ourNormal;
in vec2 texCoord;

out vec4 fragColor;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	//Diffuse
	float diff = max(dot(normal, lightDir), 0.0);

	//Specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material0.shineness * 128);

	//Final Calculations
	vec3 ambient = light.ambient * texture(material0.diffuse, texCoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material0.diffuse, texCoord).rgb;
	vec3 specular = light.specular * spec * texture(material0.specular, texCoord).rgb;

	return (ambient + diffuse + specular);
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPosition);

	//Diffuse
	float diff = max(dot(normal, lightDir), 0.0);

	//Specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material0.shineness * 128);

	//Light attenuation
	float distance = length(light.position - fragPosition);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	//Final Calculations
	vec3 ambient = light.ambient * texture(material0.diffuse, texCoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material0.diffuse, texCoord).rgb;
	vec3 specular = light.specular * spec * texture(material0.specular, texCoord).rgb;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 calculateSpotlightLight()
{

	return vec3(0.0,0.0,0.0);
}

void main()
{
	//Start calculations
	vec3 norm = normalize(ourNormal);
	vec3 viewDirection = normalize(viewPos - fragPos);

	//Directional light
	vec3 result = calculateDirectionalLight(dirLight, norm, viewDirection);
	
	//Point lights
	for(int i = 0; i < NR_OF_POINT_LIGHTS; i++)
		result += calculatePointLight(pointLight[i], norm, fragPos, viewDirection);

	//Result
    fragColor = vec4(result, 1.0);
}