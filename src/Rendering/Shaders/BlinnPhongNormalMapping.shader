#shader vertex
#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoord;
layout(location = 3) in vec3 _tangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

#define NR_OF_POINT_LIGHTS 1
uniform vec3 lightPos[NR_OF_POINT_LIGHTS];
uniform vec3 viewPos;

out VERT_SH {
	vec3 fragPos;
	vec2 texCoord;
	vec3 ourNormal;

	vec3 tangentViewPos;
	vec3 tangentFragPos;
	vec3 tangentLightPos[NR_OF_POINT_LIGHTS]; //Has to calculate that in frag shader :( - i think
} vert_sh;

void main()
{
	vert_sh.fragPos = vec3(model * vec4(pos, 1.0));
	vert_sh.texCoord = textureCoord;
	vert_sh.ourNormal = normal;

	vec3 T = normalize(vec3(model * vec4(_tangent,0.0)));
	vec3 N = normalize(vec3(model * vec4(normal,0.0)));
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	if(dot(cross(N,T), B) < 0.0) {
		T = T * -1.0;
	}

	mat3 TBN = transpose(mat3(T, B, N));
	for(int i = 0; i < NR_OF_POINT_LIGHTS; i++) {
		vert_sh.tangentLightPos[i] = TBN * lightPos[i];
	}
	vert_sh.tangentViewPos = TBN * viewPos;
	vert_sh.tangentFragPos = TBN * vert_sh.fragPos;

	gl_Position = projection * view * model * vec4(pos, 1.0);
}

#shader fragment
#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normalMap;
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
	//vec3 position;

	vec3 ambient; //Teraz kontrolujemy ambient Materialu poprzez ambient Light
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform DirectionalLight dirLight;
uniform PointLight pointLight[NR_OF_POINT_LIGHTS];

in VERT_SH {
	vec3 fragPos;
	vec2 texCoord;
	vec3 ourNormal;

	vec3 tangentViewPos;
	vec3 tangentFragPos;
	vec3 tangentLightPos[NR_OF_POINT_LIGHTS]; //Has to calculate that in frag shader :( - i think
} frag_in;

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
	vec3 ambient = light.ambient * texture(material0.diffuse, frag_in.texCoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material0.diffuse, frag_in.texCoord).rgb;
	vec3 specular = light.specular * spec * texture(material0.specular, frag_in.texCoord).rgb;

	return (ambient + diffuse + specular);
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 tangentLightPos, vec3 fragPosition, vec3 viewDir)
{
	vec3 lightDir = normalize(tangentLightPos - frag_in.tangentFragPos);

	//Diffuse
	float diff = max(dot(normal, lightDir), 0.0);

	//Specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material0.shineness * 128);

	//Light attenuation
	float distance = length(tangentLightPos - frag_in.tangentFragPos); //nwm czy to nie zamienic
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	//Final Calculations
	vec3 ambient = light.ambient * texture(material0.diffuse, frag_in.texCoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material0.diffuse, frag_in.texCoord).rgb;
	vec3 specular = light.specular * spec * texture(material0.specular, frag_in.texCoord).rgb;

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
	vec3 normal = texture(material0.normalMap, frag_in.texCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	vec3 viewDirection = normalize(frag_in.tangentViewPos - frag_in.tangentFragPos);

	//Directional light
	vec3 result = calculateDirectionalLight(dirLight, normal, viewDirection);
	
	//Point lights
	for(int i = 0; i < NR_OF_POINT_LIGHTS; i++)
		result += calculatePointLight(pointLight[i], normal, frag_in.tangentLightPos[i], frag_in.fragPos, viewDirection);

	//Result
    fragColor = vec4(result, 1.0);
}