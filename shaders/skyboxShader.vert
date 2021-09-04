/*#version 330
layout (location = 0) in vec3 position;

uniform mat4 PMatrix;
uniform mat4 VMatrix;

out vec3 TexCoords;

void main()
{
    TexCoords = position;
    gl_Position = (PMatrix * VMatrix * vec4(position, 1.0)).xyww;
} */

#version 140
uniform mat4 invPVmatrix;
in vec2 screenCoord;

out vec3 TexCoords_V;

void main(){
  vec4 farplaneCoord = vec4(screenCoord, 0.9999, 1.0);
  vec4 worldViewCoord = invPVmatrix * farplaneCoord;
  TexCoords_V = worldViewCoord.xyz / worldViewCoord.w;
  gl_Position = farplaneCoord;
}