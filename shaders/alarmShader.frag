#version 140
in vec4 alarmColor;
//in vec3 lightRayColor;
out vec4 fragColor;

void main()
{
	fragColor = vec4(alarmColor); //vystupni barva svetla
}