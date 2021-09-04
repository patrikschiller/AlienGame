	#version 140
	uniform mat4 PVMmatrix;
	uniform float cm;

	in vec3 position;
	in vec3 color;	
	in vec2 texCoord;

	smooth out vec4 theColor;
	out vec2 texCoords;
	void main() {
	  gl_Position = PVMmatrix * vec4(position, 1.0);
	  theColor = vec4(cm * /*color*/ vec3(0.4, 0.4, 0.3), 1.0);
	  texCoords = texCoord;
	}