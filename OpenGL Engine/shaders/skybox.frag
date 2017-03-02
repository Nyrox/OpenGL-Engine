#version 330 core

in vec3 TexCoords;
out vec4 fragColor;

uniform samplerCube skybox;

void main()
{    
    vec3 color = texture(skybox, TexCoords).rgb;
	
	fragColor = vec4(color, 1.0);
}