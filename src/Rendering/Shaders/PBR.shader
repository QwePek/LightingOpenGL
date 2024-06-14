#shader vertex
#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoord;
layout(location = 3) in vec3 _tangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

out vec3 fragPos;
out vec3 ourNormal;
out vec2 texCoord;

void main()
{
	fragPos = vec3(model * vec4(pos, 1.0));
	ourNormal = normalMatrix * normal;
	texCoord = textureCoord;

	gl_Position = projection * view * vec4(fragPos, 1.0);
}

#shader fragment
#version 330 core
struct Material {
	sampler2D albedoMap;
	sampler2D normalMap;
	sampler2D metallicMap;
	sampler2D roughnessMap;
	sampler2D aoMap;
};
uniform Material material0;
uniform samplerCube irradianceMap;
// uniform samplerCube prefilterMap;
// uniform sampler2D brdfLUT;

struct DirectionalLight {
	vec3 direction;
	vec3 color;
};

#define NR_OF_POINT_LIGHTS 4
struct PointLight {
	vec3 position;
	vec3 color;
};

uniform DirectionalLight dirLight;
uniform PointLight pointLight[NR_OF_POINT_LIGHTS];
uniform vec3 viewPos;

in vec3 fragPos;
in vec3 ourNormal;
in vec2 texCoord;

out vec4 fragColor;


const float PI = 3.14159265359;

vec3 getNormalFromNormalMap()
{
	vec3 tangentNormal = texture(material0.normalMap, texCoord).xyz * 2.0 - 1.0;

	vec3 Q1 = dFdx(fragPos);
	vec3 Q2 = dFdy(fragPos);
	vec2 st1 = dFdx(texCoord);
	vec2 st2 = dFdy(texCoord);

	vec3 N = normalize(ourNormal);
	vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
	vec3 B = -normalize(cross(N, T));
	mat3 TBN = mat3(T, B, N);

	return normalize(TBN * tangentNormal);
}

//N- normal, H- halfway vector, a- surface roughness
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float aSqr = a*a;
	float nDotH = max(dot(N, H), 0.0);
	float nDotH2 = nDotH * nDotH;
	
	float denominator = (nDotH2 * (aSqr - 1.0) + 1.0);
	denominator = PI * denominator * denominator;

	return aSqr / denominator;
}

//Aproksymuje probabilistycznie rough surface gdzie micro surface details overshadow each other
float GeometrySchlickGGX (float NDotV, float roughness)
{
	float r = roughness + 1.0;
	float k = (r*r) / 8.0;

	float denominator = (NDotV) * (1.0 - k) + k;

	return NDotV / denominator;
}

//To ze Schlikiem aproksymuje
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NDotV = max(dot(N, V), 0.0);
	float NDotL = max(dot(N, L), 0.0);

	return GeometrySchlickGGX(NDotV, roughness) * GeometrySchlickGGX(NDotL, roughness);
}

vec3 frenselSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 frenselSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 N, vec3 viewDir, float roughness, float metallic, vec3 F0, vec3 albedo)
{
	vec3 L = normalize(-light.direction);
	vec3 H = normalize(viewDir + L);
	
	vec3 radiance = light.color;

	float NDF = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, viewDir, L, roughness);
	vec3 F = frenselSchlick(clamp(dot(H, viewDir), 0.0, 1.0), F0);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(N, viewDir), 0.0) * max(dot(N, L), 0.0) + 0.0001;
	vec3 specular = numerator / denominator;

	float NdotL = max(dot(N, L), 0.0);
	return (kD * albedo / PI + specular) * radiance * NdotL;
}

vec3 calculatePointLight(PointLight light, vec3 N, vec3 viewDir, float roughness, float metallic, vec3 F0, vec3 albedo)
{
	vec3 L = normalize(light.position - fragPos);
	vec3 H = normalize(viewDir + L);
	
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (distance * distance);
	vec3 radiance = light.color * attenuation;

	float NDF = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, viewDir, L, roughness);
	vec3 F = frenselSchlick(max(dot(H, viewDir), 0.0), F0);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(N, viewDir), 0.0) * max(dot(N, L), 0.0) + 0.0001;
	vec3 specular = numerator / denominator;

	float NdotL = max(dot(N, L), 0.0);
	return (kD * albedo / PI + specular) * radiance * NdotL;
}

void main()
{
	//Start calculations
	vec3 albedo =	  pow(texture(material0.albedoMap, texCoord).rgb, vec3(2.2));
	float metallic =  texture(material0.metallicMap, texCoord).r;
	float roughness = texture(material0.roughnessMap, texCoord).r;
	float ao =		  texture(material0.aoMap, texCoord).r;

	vec3 N = getNormalFromNormalMap();
	vec3 V = normalize(viewPos - fragPos);
	vec3 R = reflect(-V, N);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);
	
	vec3 L0 = vec3(0.0);

	L0 += calculateDirectionalLight(dirLight, N, V, roughness, metallic, F0 ,albedo);
	//Point lights
	for(int i = 0; i < NR_OF_POINT_LIGHTS; i++)
		L0 += calculatePointLight(pointLight[i], N, V, roughness, metallic, F0 ,albedo);

	// vec3 F = frenselSchlick(max(dot(N, V), 0.0), F0);
	// vec3 kS = F;
	// vec3 kD = 1.0 - kS;
	// kD *= 1.0 - metallic;
	vec3 kS = frenselSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
	vec3 kD = 1.0 - kS;
	vec3 irradiance = texture(irradianceMap, N).rgb;
	vec3 diffuse = irradiance * albedo;
	vec3 ambient = (kD * diffuse) * ao;

	// const float MAX_REFLECTION_LOD = 4.0;
	// vec3 prefilteredColor = textureLod(prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
	// vec2 brdf = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
	// vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);
	
	//vec3 ambient = (kD + diffuse) * ao;
	//vec3 ambient = vec3(0.04) * albedo * ao;
	vec3 color = ambient + L0;

	//HDR
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0 / 2.2));

	//Result
	fragColor = vec4(color, 1.0);
}