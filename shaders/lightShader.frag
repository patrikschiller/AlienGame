#version 140
in vec4 sunColor;
in vec3 lightRayColor;
out vec4 fragColor;

void main()
{
	vec4 snclr = vec4(0.5f, 0.0f, 0.5f, 1.0f);
	fragColor = vec4(sunColor); //vystupni barva svetla
}