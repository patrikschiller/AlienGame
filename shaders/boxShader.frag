#version 140

smooth in vec4 theColor; // vstupní modifier barva
in vec2 oTexCoord; //texturovací souøadnice
uniform sampler2D ourTexture; // texturovací jednotka
/*===========*/
in vec3 Normal; //normála vrcholu
in vec3 fragPos; //pozice fragmentu
in vec3 sunLoc; // pozice slunce <global illumination>
in vec3 viewerPos; // pozice 1st person pohledu (kamery)

struct Material{ //material objektu
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

struct DirectionalLight{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight{ //klasicke svetlo
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constantPower;
	float linearPower;
	float quadraticPower;
};
#define POINT_LIGHTS 6
uniform PointLight pointLights[POINT_LIGHTS];

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
#define ALARMS 1
uniform Reflector alarms[ALARMS];

uniform DirectionalLight dirLight; //DATA!!
uniform PointLight pointLight;
uniform Reflector reflector; //DATA!!
/*========*/

//vec4 lightColor = vec4(1.0, 1.0, 0.0f, 0.1f); // hardcoded barva slunce

out vec4 outputColor; //výstupní barva fragmentu

vec4 phongDirectional(DirectionalLight DL, vec3 normal, vec3 viewDir){
	vec3 ambient = DL.ambient * material.ambient;

	vec3 lightDirection = normalize(-DL.direction);
	float diffuseAngle = max(dot(normal, lightDirection), 0.0f);
	vec3 diffuse = DL.diffuse * diffuseAngle * material.diffuse;

	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specularAngle = pow(max(dot(viewDir, reflectDirection),0.0f), material.shininess);
	vec3 specular = DL.specular * specularAngle * material.specular;

	vec3 phongSum = (ambient + diffuse + specular);
	return vec4(phongSum, 1.0f);
}

vec4 phongMultiDirectional(PointLight PL, vec3 Normal, vec3 fragPos, vec3 viewDir){
	float fragmentDistance = length(pointLight.position - fragPos);
	float attenuation = 1.0/(pointLight.constantPower + pointLight.linearPower * fragmentDistance + pointLight.quadraticPower * (fragmentDistance * fragmentDistance));
	//Ambient
	float strength = 0.1;
	vec4 ambient = vec4(material.ambient, 1.0f) /*strength*/ * vec4(pointLight.ambient, 1.0f);

	// Diffuse
	vec3 normalized = normalize(Normal);

	vec3 lightDir = normalize(pointLight.position - fragPos);
	//vec3 lightDir  = normalize(vec3(light.direction.x, light.direction.y, -light.direction.z));

	float diffuseAngle = max(dot(normalized, lightDir), 0.0);
	vec4 diffuse = diffuseAngle * vec4(material.diffuse, 1.0f) * vec4(pointLight.diffuse, 1.0f);

	// Specular
	////vec3 viewDir = normalize(viewerPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normalized);
	float specularAngle = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //shinines
	vec4 specular =/* specStrength*/ (vec4(material.specular, 1.0f) * specularAngle) * vec4(pointLight.specular, 1.0f);
	if(length(viewerPos) == 0.0f){
		specular = vec4(0.1f, 0.1f, 0.1f, 1.0f);
	}

	//Phong sum
	vec4 phong = ambient + diffuse*attenuation + specular*attenuation;
	return phong;
}

vec4 phongReflector(Reflector RF, vec3 Normal, vec3 fragPos){
	// vec3 reflectorPosition = RF.position;
	//Ambient
	vec3 ambient = material.ambient * RF.ambient;

	// Diffuse
	vec3 normalized = normalize(Normal);
	vec3 lightDir = normalize(/*reflector.position*/ /*viewerPos*//*reflectorPosition*/RF.position - fragPos/*RF.direction*/);
	float diffuseAngle = max(dot(normalized, lightDir), 0.0);
	vec3 diffuse = diffuseAngle * material.diffuse * /*reflector.diffuse*/ /*pointLight.diffuse*/ RF.diffuse;

	// Specular
	vec3 viewDir = normalize(viewerPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normalized);
	float specularAngle = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //shininesW
	vec3 specular =/* specStrength*/ material.specular * specularAngle * /*reflector.specular*//*pointLight*/RF.specular;

	// Reflector smoothness
	//vec3 testDir = vec3(-1.0f, 0.0f, 0.0f);
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
/*vec4 phongAlarm(Reflector RF){
	vec4 color;

	vec3 lightDir = normalize(RF.position - fragPos);
	float theta = dot(lightDir, normalize(-RF.direction));
	if(theta > RF.cutOff){
	//ambient
		vec3 ambient = material.ambient * RF.ambient;
		// Diffuse
		vec3 normalized = normalize(Normal);
		vec3 lightDir = normalize(RF.position - fragPos);
		float diffuseAngle = max(dot(normalized, lightDir), 0.0);
		vec3 diffuse = diffuseAngle * material.diffuse *  RF.diffuse;

		color = vec4((ambient + diffuse), 1.0f);
	}else{
		color = vec4(0.0f, 1.0f, 0.0f, 1.0);
	}
	return color;
}*/

//hradcoded fog:
float near = 0.1f;
float far = 100.0f;

//uniform mat4 PVM;
float depthLinearization(float depth){
	float z_val = depth * 2.0f - 1.0f; // -device coordinates
	return (2.0f * near * far) / (far + near - z_val * (far - near));
}

void main() {
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewerPos - fragPos);
	//directional light
	vec4 resultLight = phongDirectional(dirLight, normal, viewDir);

	//multidirectional (point) light
	vec4 phongMulti = phongMultiDirectional(pointLight, normal, fragPos, viewDir);

	//reflector light
	vec4 phongReflect = phongReflector(reflector, normal, fragPos);

	vec4 objectColor = texture(ourTexture, oTexCoord)/* theColor*/ /* vec4(altColor, 1.0f)*/;

	vec4 lightResult = phongMulti + phongReflect + resultLight;//pata je nejvetsi sikula a nejaky alarm zvladne levou zadni
	for(int i = 0; i < ALARMS; i++){
	if(alarms[i].calculate)
		lightResult += phongReflector(alarms[i], normal, fragPos);
		//lightResult += phongAlarm(alarms[i]);
	}

	vec4 result = lightResult * objectColor;

	/** FOG */
	float depth = depthLinearization(gl_FragCoord.z) / far;
	vec3 fogColor = vec3(0.1f, 0.15f, 0.2f);
	result += vec4(vec3(depth * fogColor.x , depth * fogColor.y, depth * fogColor.z), 1.0);

	outputColor = vec4(result);

}
