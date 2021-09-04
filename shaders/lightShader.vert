#version 330
uniform mat4 PVMmatrix;//PVM matice
uniform vec3 lightSourceColor;
uniform vec3 color;	 // barva

layout (location = 0) in vec3 position; //pozice	

//in vec2 texCoord; Zatim ne

out vec4 sunColor;
out vec3 lightRayColor;
//out vec2 oTexCoord; Zatim ne

void main() {
	vec3 tmpClr = vec3(1.0f, 1.0f, 0.0f);
	gl_Position = PVMmatrix * vec4(position, 1.0);
	sunColor = vec4(tmpClr, 1.0);
	lightRayColor = lightSourceColor;
	//oTexCoord = texCoord;  Zatim ne
}