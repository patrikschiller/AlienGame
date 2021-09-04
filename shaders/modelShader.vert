#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 fragPos;
out vec4 fogAddition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normalMatrix;

void main() {
	TexCoords = texCoords;
	gl_Position = projection * view * model * vec4(position, 1.0);
	Normal = normalize(vec3(normalMatrix * vec4(normal,1.0f)));

	fragPos = vec3(model * vec4(position, 1.0f));

	//gl_Position = PVM * vec4(position, 1.0);
	//gl_Position = PVMmatrix * vec4(position, 1.0);
	//texCoord = texCoords;
}