#version 140
uniform mat4 PVMmatrix;//PVM matice
//uniform vec3 lightSourceColor;
uniform vec3 color;	 // barva

in vec3 position; //pozice	

//in vec2 texCoord; Zatim ne

out vec4 alarmColor;
//out vec3 lightRayColor;
//out vec2 oTexCoord; Zatim ne

void main() {
	gl_Position = PVMmatrix * vec4(position, 1.0);
	alarmColor = vec4(color, 1.0);
	//lightRayColor = lightSourceColor;
	//oTexCoord = texCoord;  Zatim ne
}