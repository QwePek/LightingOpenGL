#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;

out vec3 localPos;
void main()
{
    localPos = aPos;
    mat4 rotView = mat4(mat3(view)); //usuwa translacje z view
    vec4 clipPos = projection * rotView * vec4(localPos, 1.0);

    gl_Position = clipPos.xyww;
}

#shader fragment
#version 330 core

in vec3 localPos;
out vec4 FragColor;

uniform samplerCube enviromentMap;

void main()
{
    vec3 envColor = textureLod(enviromentMap, localPos, 4).rgb;
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0 / 2.2));

    FragColor = vec4(envColor, 1.0);
}