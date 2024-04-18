#shader vertex
#version 330 core
layout(location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0f);
}

#shader fragment
#version 330 core
uniform vec3 lightColor;
out vec4 fragColor;

void main()
{
    fragColor = vec4(lightColor, 1.0);
}