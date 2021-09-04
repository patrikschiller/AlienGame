	#version 140
	in vec2 texCoords;
	uniform sampler2D texSampler;
	smooth in vec4 theColor;

	//hardcoded fog:
	float near = 0.1f;
	float far = 50.0f;

	//uniform mat4 PVM;
	float depthLinearization(float depth){
		float z_val = depth * 2.0f - 1.0f; // -device coordinates
		return (2.0f * near * far) / (far + near - z_val * (far - near));
	}
	out vec4 outputColor;
	void main() {
	  //outputColor = theColor * 0.8f;
	  outputColor = theColor * (0.89f * vec4(texture(texSampler, vec2(texCoords.x + 0.25f,texCoords.y + 0.25f))) + 0.99f * vec4(texture(texSampler, vec2(texCoords.x + 0.25f + 0.25f,texCoords.y + 0.25f))));
	  	/** FOG - linear*/
		float depth = depthLinearization(gl_FragCoord.z) / far;
		vec3 fogColor = vec3(0.1f, 0.15f, 0.2f) * 1.4f;
		outputColor += vec4(vec3(depth * fogColor.x , depth * fogColor.y, depth * fogColor.z), 1.0);

	}