#version 330

//uniform float time;         //V 
uniform mat4 Vmatrix;         //V
uniform sampler2D texSampler;   //V

smooth in vec3 position_v;    //V
smooth in vec2 texCoord_v;    //V

out vec4 FragColor;            

uniform ivec2 pattern/* = ivec2(12, 6)*/;
uniform int actualFrame;
uniform int animationType; //1 - multi image , 2 - moving texture
uniform float time;


vec4 multiFrame() {

  vec2 offset = vec2(1.0) / vec2(pattern);

  vec2 texCoordBase = texCoord_v / vec2(pattern);
  vec2 texCoord = texCoordBase + vec2(actualFrame % pattern.x, (actualFrame / pattern.x)) * offset;

  return texture(texSampler, texCoord);
}

vec4 movingTexture()
{
	//vec2 texCoord = vec2(cos(time)*texCoord_v.x, sin(time)*texCoord_v.y);
	//vec2 texCoord = vec2(tan(time)*(texCoord_v.x + 0.5f), tan(time)*(texCoord_v.y + 0.5f));
	//vec2 texCoord = vec2(cos(time)*(texCoord_v.x)*sin(time), cos(time)*(texCoord_v.y)*sin(time));
	mat2 rot = mat2(cos(time/4), sin(time/4), -sin(time/4), cos(time/4));
	//mat2 scale = mat2((10.0f + cos(time/4))/11, 0.0f, (10.0f + cos(time/4))/11, 0.0f);
	//mat2 scale = mat2((10.0f + cos(time/4))/9.0f, 0.0f, (10.0f + cos(time/4))/9.0f, 0.0f);
	vec2 scale = vec2( (10.0f + cos(time/4))/9.0f, (10.0f + cos(time/4))/9.0f );
	vec2 texCoord = texCoord_v * 2 - 1.0f; //Transform from [0, 1] coords to [-1, 1] coords
	texCoord = rot * texCoord;
	texCoord = scale * texCoord;
	texCoord = (texCoord + 1.0f)/2; //Inverse tranform to [0, 1] coords
	if(texCoord.x > 1.0f || texCoord.y > 1.0f || texCoord.x < 0.0f || texCoord.y < 0.0f){
		texCoord = vec2(0.0f, 0.0f);
	}
    return texture(texSampler, texCoord) * vec4(abs(sin(time)), abs(cos(time)), abs(sin(time)), 1.0f);
}
void main() {
	switch(animationType){
		case 1:
			FragColor = multiFrame();
			break;
		case 2:
			FragColor = movingTexture();
			break;
		default:
		    FragColor = multiFrame();
			break;
	}
  //FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}
