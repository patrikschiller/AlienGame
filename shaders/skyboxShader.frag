/*#version 330
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    //FragColor = texture(skybox, TexCoords) + vec4(1.0f, 0.0f, 1.0f, 1.0f);
	FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}*/

#version 140

uniform samplerCube skybox;

in vec3 TexCoords_V;
out vec4 FragColor;

//hradcoded fog:
float near = 0.1f;
float far = 75.0f;

//uniform mat4 PVM;
float depthLinearization(float depth){
	float z_val = depth * 2.0f - 1.0f; // -device coordinates
	return (2.0f * near * far) / (far + near - z_val * (far - near));
}

void main(){
	FragColor = texture(skybox, TexCoords_V);

	
	/** FOG - Linear*/
	float depth = depthLinearization(gl_FragCoord.z) / far;
	vec3 fogColor = vec3(0.1f, 0.15f, 0.2f);
	FragColor += vec4(vec3(depth * fogColor.x , depth * fogColor.y, depth * fogColor.z), 1.0);
}

