#shader vertex
#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor; // output a color to the fragment shader

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0f);
	ourColor = color;
}

#shader fragment
#version 330 core
in vec3 ourColor;
out vec4 fragColor;

void main()
{
	//fragColor = mix(texture(ourTexture1, texCoord), texture(ourTexture2, vec2(1.0f - texCoord.x, texCoord.y)), 0.2f) *
		//vec4(verColor, 1.0f);
    fragColor = vec4(ourColor, 1.0);
}