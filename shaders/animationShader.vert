#version 330

uniform mat4 PVMmatrix;    

layout (location = 0) in vec3 position;           
layout (location = 1) in vec2 texCoord;           

smooth out vec2 texCoord_v; 

void main() {

  gl_Position = PVMmatrix * vec4(position, 1.0f);  
  
  texCoord_v = texCoord;
}