#version 140

//Attributy
in vec3 position; //pozice
in vec3 color;	 // barva	
in vec2 texCoord;
in vec3 normal;

//Uniformy
uniform mat4 PVMmatrix;//PVM matice
uniform mat4 VMatrix;
uniform mat4 MMatrix;
uniform mat4 normalMatrix;
uniform float cm; // color modifier
uniform vec3 sunLocation;
uniform vec3 viewerLocation;
uniform float time; 

//Vystupy
//smooth out vec4 theColor;
out vec2 oTexCoord;

out vec3 Normal;
out vec3 fragPos;
out vec3 sunLoc;
out vec3 viewerPos;

void main() {
	//kamerove souradnice
	//vec3 vertexPosition = (VMatrix * MMatrix * vec4(position, 1.0f)).xyz;
	//vec3 vertexNormal = normalize((VMatrix * normalMatrix * vec4(normal, 0.0f))).xyz;

	gl_Position = PVMmatrix * vec4(position, 1.0);

	oTexCoord = texCoord;
	Normal = vec3(normalMatrix * vec4(normal,1.0f));
	fragPos = vec3(MMatrix * vec4(position, 1.0f));
	sunLoc = sunLocation;
	viewerPos = viewerLocation;
}

