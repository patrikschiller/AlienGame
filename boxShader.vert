#version 140

uniform mat4 PVMmatrix;//PVM matice
uniform float cm; // color modifier
in vec3 position; //pozice
in vec3 color;	 // barva	
in vec2 texCoord;

smooth out vec4 theColor;
out vec2 oTexCoord;

void main() {
	gl_Position = PVMmatrix * vec4(position, 1.0);
	theColor = vec4(cm * color, 1.0);
	oTexCoord = texCoord;
}
	//#version 140
	/*uniform mat4 PVMmatrix;
	uniform float cm;
	in vec3 position;
	in vec3 color;\
	smooth out vec4 theColor;
	void main() {
	  gl_Position = PVMmatrix * vec4(position, 1.0);
	  theColor = vec4(cm * color, 1.0);
	}*/
