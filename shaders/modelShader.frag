
/*
#version 330
in vec2 TexCoords //V
in vec3 Normal; //V

out vec4 FragColor;

uniform sampler2D texture_diffuse1; //V
uniform sampler2D texture_specular1; //V
uniform vec4 color; //V

void main()
{
	FragColor = texture(texture_diffuse1, TexCoords) + color + texture(texture_specular1, TexCoords);
}*/

#version 330
//smooth in vec4 theColor; // vstupní modifier barva
in vec2 TexCoords; //texturovací souøadnice
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec4 color;
uniform bool useTextures;
/*===========*/
in vec3 Normal; //normála vrcholu
in vec3 fragPos; //pozice fragmentu
//in vec3 sunLoc; // pozice slunce <global illumination>
uniform vec3 viewerPos; // pozice 1st person pohledu (kamery)

struct Material{ //material objektu  //NOT SET
	vec3 ambient;//NOT SET
	vec3 diffuse;//NOT SET
	vec3 specular;//NOT SET
	float shininess;//NOT SET
};
uniform Material material;
struct Reflector{ //parametry reflektoru
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float cutOff;
	float outerCutoff;

	float constantPower;
	float linearPower;
	float quadraticPower;

	bool calculate;
};
struct PointLight{
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constantPower;
	float linearPower;
	float quadraticPower;
};
struct DirectionalLight{
	vec3 direction;
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform vec3 rfl;
uniform Reflector reflectors[1];
#define ALARMS 3
uniform Reflector alarms[ALARMS];
uniform PointLight pointlights[1];
uniform DirectionalLight dirLights[1];

out vec4 FragColor;

vec4 phongReflector(Reflector RF, vec3 Normal, vec3 fragPos){
	//Ambient
	vec3 ambient;
	if(useTextures){
		ambient = vec3(texture(texture_diffuse1, TexCoords)) * RF.ambient;
	}else{
		ambient = material.ambient * RF.ambient;
	}

	// Diffuse
	vec3 normalized = normalize(Normal);
	vec3 lightDir = normalize(RF.position - fragPos);
	float diffuseAngle = max(dot(normalized, lightDir), 0.0);
	vec3 diffuse;
	if(useTextures){
		diffuse = diffuseAngle * vec3(texture(texture_diffuse1, TexCoords)) *  RF.diffuse;
	}else{
		diffuse = diffuseAngle * material.diffuse *  RF.diffuse;
		//diffuse = vec3(0.0f, 1.0f, 0.0f);
		//diffuse = diffuseAngle * vec3(0.0f, 1.0f, 0.0f) *  RF.diffuse;
	}

	// Specular
	vec3 viewDir = normalize(viewerPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normalized);
	float specularAngle = pow(max(dot(viewDir, reflectDir), 0.0),/* material.shininess*/12); //shininesW
	vec3 specular;
	if(useTextures){
		specular = vec3(texture(texture_specular1, TexCoords)) * specularAngle * RF.specular;
	}else{
		specular = material.specular * specularAngle * RF.specular;
		//specular = vec3(0.0f, 0.0f, 1.0f);
		//specular = vec3(0.0f, 0.0f, 1.0f) * specularAngle * RF.specular;
	}

	// Reflector smoothness
	float theta = dot(lightDir, normalize(-RF.direction));
	float delta = (RF.cutOff - RF.outerCutoff);
	float lightIntensity = clamp((theta - RF.outerCutoff) / delta, 0.0f, 1.0f);
	specular *= lightIntensity;
	diffuse *= lightIntensity;

	// Light attenuation
	float fragmentDistance = length(RF.position - fragPos);
	float attenuation = 1.0/(/*reflector.constantPower*/ /*pointLight.constantPower*/ RF.constantPower + /*reflector.linearPower*//* pointLight.linearPower */RF.linearPower
					* fragmentDistance + /*reflector.quadraticPower *//* pointLight.quadraticPower*/ RF.quadraticPower
					* (fragmentDistance * fragmentDistance));
	//Phong sum
	vec3 phong = ambient + diffuse * attenuation + specular * attenuation;
	return vec4(phong, 1.0f);
}

//===========================================================

vec4 phongMultiDirectional(PointLight PL, vec3 Normal, vec3 fragPos, vec3 viewDir){
	float fragmentDistance = length(PL.position - fragPos);
	float attenuation = 1.0/(PL.constantPower + PL.linearPower * fragmentDistance + PL.quadraticPower * (fragmentDistance * fragmentDistance));
	//Ambient
	float strength = 0.1;
	vec4 ambient;
	if(useTextures){
		ambient =/* vec4(material.ambient, 1.0f)*/ /*strength*/ vec4(texture(texture_diffuse1, TexCoords)) * vec4(PL.ambient, 1.0f);
	}else{
		if(length(material.ambient) > 0.0f){
			ambient = vec4(material.ambient, 1.0f) * vec4(PL.ambient, 1.0f);
		}else{
			ambient = vec4(0.1f,0.1f,0.1f, 1.0f) * vec4(PL.ambient, 1.0f);
		}
	}

	// Diffuse
	vec3 normalized = normalize(Normal);

	vec3 lightDir = normalize(PL.position - fragPos);
	//vec3 lightDir  = normalize(vec3(light.direction.x, light.direction.y, -light.direction.z));

	float diffuseAngle = max(dot(normalized, lightDir), 0.0);
	vec4 diffuse;
	if(useTextures){
		diffuse = diffuseAngle * /*vec4(material.diffuse, 1.0f)*/vec4(texture(texture_diffuse1, TexCoords)) * vec4(PL.diffuse, 1.0f);
	}else{
		if(length(material.diffuse) > 0.0f){//Loï, modely s materiálem - materiál naèten
			diffuse = vec4(PL.diffuse, 1.0f) * diffuseAngle * vec4(material.diffuse, 1.0f);
		}else{//Základna, materiály nenaèteny
			diffuse = vec4(PL.diffuse, 1.0f) * diffuseAngle * vec4(0.99f, 0.3f, 0.3f, 1.0f);
		}
	}

	// Specular
	////vec3 viewDir = normalize(viewerPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normalized);

	float shine;
	if(useTextures){
		shine = material.shininess;
	}else{
		shine = 16.0f;
	}

	float specularAngle = pow(max(dot(viewDir, reflectDir), 0.0), shine); //shinines
	vec4 specular;
	if(useTextures){
		specular =/* specStrength*/ (/*vec4(material.specular, 1.0f)*/vec4(texture(texture_specular1, TexCoords)) * specularAngle) * vec4(PL.specular, 1.0f);
	}else{
		if(length(material.specular) > 0.0f){//Loï, modely s materiálem - materiál naèten
			specular = vec4(PL.specular, 1.0f) * specularAngle * vec4(material.specular, 1.0f);
		}else{//Základna, materiály nenaèteny
			specular = vec4(PL.specular, 1.0f) * specularAngle * vec4(0.1f, 0.1f, 0.1f, 1.0f);
		}
	}
	if(length(viewerPos) == 0.0f){ //For testing
		specular = (vec4(material.specular, 1.0f) * specularAngle) * vec4(0.1f, 0.1f, 0.1f, 1.0f);
	}

	//Phong sum
	vec4 phong = ambient + diffuse*attenuation + specular*attenuation;
	return phong;
}


//===========================================================


vec4 phongDirectional(DirectionalLight DL, vec3 normal, vec3 viewDir){

	vec4 ambient;
	if(useTextures){
		ambient = vec4(DL.ambient, 1.0f) *  vec4(texture(texture_diffuse1, TexCoords)) ;
	}else{
		if(length(material.ambient) > 0.0f){
			ambient = vec4(material.ambient, 1.0f) * vec4(DL.ambient, 1.0f);
		}else{
			if(length(material.ambient) > 0.0f){
				ambient = vec4(material.ambient, 1.0f) * vec4(DL.ambient, 1.0f);	
			}else{
				ambient = vec4(0.05f,0.05f,0.1f, 1.0f)*0.2f * vec4(DL.ambient, 1.0f) * 0.6f;
			}
		}
	}

	vec3 lightDirection = normalize(DL.position);
	float diffuseAngle = max(dot(normal, lightDirection), 0.0f);
	vec4 diffuse;
		if(useTextures){
		diffuse = vec4(DL.diffuse, 1.0f) * diffuseAngle * vec4(texture(texture_diffuse1, TexCoords));
	}else{
		if(length(material.diffuse) > 0.0f){//Loï, modely s materiálem
			diffuse = vec4(DL.diffuse, 1.0f) * diffuseAngle * vec4(material.diffuse, 1.0f); //CORRECT
		}else{//Základna - modely bez naèteného materiálu
			diffuse = vec4(DL.diffuse, 1.0f) * diffuseAngle * vec4(0.13f, 0.13f, 0.13f, 1.0f); //CORRECT
		}
	}

	float shine;
	if(useTextures){
		shine = material.shininess;
	}else{
		shine = 8.0f;
	}
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specularAngle = pow(max(dot(viewDir, reflectDirection),0.0f), shine);
	vec4 specular;
	if(useTextures){
		specular = vec4(texture(texture_specular1, TexCoords)) * specularAngle * vec4(DL.specular, 1.0f);
	}else{
		if(length(material.specular) > 0.0f){//Loï	
			specular = vec4(DL.specular, 1.0f) * specularAngle * vec4(material.specular,1.0f); //CORRECT
		}else{//Základna a dalsi objekty bez materialu
			specular = vec4(DL.specular, 1.0f) * specularAngle * vec4(0.1f, 0.1f, 0.1f, 1.0f); //CORRECT
		}
	}


	vec4 phongSum = (ambient + diffuse + specular);
	if(length(DL.specular) == 0.0f && length(DL.diffuse) == 0.0f && length(DL.ambient) == 0.0f){
		phongSum = vec4(1.0f, 0.0f, 0.0f,1.0f);
	}
	return vec4(phongSum);
}

//hardcoded fog:
float near = 0.1f;
float far = 50.0f;

//uniform mat4 PVM;
float depthLinearization(float depth){
	float z_val = depth * 2.0f - 1.0f; // -device coordinates
	return (2.0f * near * far) / (far + near - z_val * (far - near));
}
float exponencialFog(float depth){
	float z_val = depth * 2.0f - 1.0f; 
	float density = 0.9f;
	return exp(-(density * z_val));
}

void main()
{

	/** FOG - linear*/
	float depth = depthLinearization(gl_FragCoord.z) / far;
	vec3 fogColor = vec3(0.1f, 0.15f, 0.2f) * 1.4f;
	FragColor += vec4(vec3(depth * fogColor.x , depth * fogColor.y, depth * fogColor.z), 1.0);


	vec3 viewDir = normalize(viewerPos - fragPos);
	for(int i = 0; i < 1; i++){
		FragColor += phongDirectional(dirLights[i], Normal, viewDir);  //OK
	}
	for(int i = 0; i < 1; i++){
		//FragColor += phongReflector(reflectors[i], Normal, fragPos); //WORKS NICE
	}

	for(int i = 0; i < 1; i++){
		FragColor += phongMultiDirectional(pointlights[i], Normal, fragPos, viewDir); //OK
	}
	if(!useTextures && color.x > 0.0f && color.y > 0.0f && color.z > 0.0f){
		FragColor *= color;
	}
	//Alarms
	for(int i = 0; i < ALARMS; i++){
		if(length(alarms[i].ambient) > 0.0f)
			FragColor += phongReflector(alarms[i], Normal, fragPos);
	}

	/** FOG - exponencial - postprocess*/
	depth = exponencialFog(gl_FragCoord.z);
	//FragColor = vec4(texture(texture_diffuse1, TexCoords)) + vec4(texture(texture_specular1, TexCoords)) ;
	//FragColor = depth * FragColor + (1-depth) * vec4(fogColor, 1.0f);
	//FragColor += vec4(vec3(depth * fogColor.x , depth * fogColor.y, depth * fogColor.z), 1.0);
}