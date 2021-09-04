#include "pch.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include "pgr.h"
#include "data.h"
#include "shadersData.h"
#include "spline.h"
#include "render.h"
#include "Program.h"
#include "Camera.h"
#include "Model.h"
//#include "filesystem.h"

MeshGeometry* boxGeometry = NULL;
MeshGeometry* squareGeometry = NULL;
MeshGeometry* sunGeometry = NULL;
MeshGeometry* alarmGeometry = NULL;
MeshGeometry* skyboxGeometry = NULL;

const char* ASTEROID_MODEL_NAME = "data/FortVertices1.obj";
const char* WALL_TEXTURE = "data/textures/wall.jpg";

//hlavni shader program
SCommonShaderProgram shaderProgram;
Program * boxProgram;
Program * sunProgram;
Program * alarmProgram;
Program * modelProgram2;
Program * skyboxProgram;
Program * squareProgram;
std::vector<Program> shaderPrograms;

struct boxShaderProgram {
	// identifier for the program
	Program program;              // = 0;
	// vertex attributes locations
	GLint posLocation;           // = -1;
	GLint colorLocation;
	GLint normalLocation;
	GLint texCoordLocation;      // = -1;
	// uniforms locations
	GLint PVMmatrixLocation;     // = -1;
	GLint VmatrixLocation;      //view/camera matrix
	GLint MmatrixLocation;		// modelovaci matice
	GLint normalMatrixLocation;

	// material 
	GLint diffuseLocation;    // = -1;
	GLint ambientLocation;    // = -1;
	GLint specularLocation;   // = -1;
	GLint shininessLocation;  // = -1;

	// light
	GLint lightAmbientLocation;
	GLint lightDiffuseLocation;
	GLint lightSpecularLocation;

	GLint texSamplerLocation;    // = -1;

	GLint colorModifierLocation;

	//light source
	GLint lightSourceColorLocation;
	GLint lightSourcePositionLocation;
	GLint lightSourceDirectionLocation;
	GLint lightDirectionLocation;
	GLint lightPositionLocation;

	GLint lightConstantPowerLocation;
	GLint lightLinearPowerLocation;
	GLint lightQuadraticPowerLocation;

	GLint reflectorPositionLocation;
	GLint reflectorDirectionLocation;

	GLint reflectorCutoffLocation;
	GLint reflectorOuterCutoffLocation;

	GLint sunLocation;
	GLint viewerLocation;

} boxShaderProgram;

struct sunShaderProgram {
	// identifier for the program
	Program program;

	GLint posLocation;           // = -1;
	GLint colorLocation;
	GLint texCoordLocation;      // = -1;

	GLint PVMmatrixLocation;     // = -1;
	GLint texSamplerLocation;    // = -1;

	GLint lightSourceColorLocation;

}sunShaderProgram;

struct alarmShaderProgram {
	Program program;

	GLint posLocation;
	GLint colorLocation;
	GLint texCoordLocation; //no used

	GLint PVMmatrixLocation;
	GLint texSamplerLocation; //not used
} alarmShaderProgram;

struct skyboxShaderProgram {
	Program * program;

	GLint posLocation;
	GLint VmatrixLocation;
	GLint PmatrixLocation;
	GLint screenCoordLocation;
	GLint PVmatrixLocation;

	GLint texSamplerLocation;
}skyboxShaderProgram;

// ==== UNIFORMY

//PVM matice

void setTransformUniforms(const glm::mat4 &modelMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) {
	glm::mat4 PVM = projectionMatrix * viewMatrix * modelMatrix;
	glUniformMatrix4fv(shaderProgram.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));

	//glUniformMatrix4fv(boxShaderProgram.VmatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	//glUniformMatrix4fv(boxShaderProgram.MmatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	//glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));
	//glUniformMatrix4fv(boxShaderProgram.normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));  // correct matrix for non-rigid transform
}
void setAttributesUniforms(GLuint texture) {
	if (texture != 0) {
		glUniform1i(shaderProgram.useTextureLocation, 1);  // do texture sampling
		glUniform1i(shaderProgram.texSamplerLocation, 0);  // texturing unit 0 -> samplerID   [for the GPU linker]
		glActiveTexture(GL_TEXTURE0 + 0);                  // texturing unit 0 -> to be bound [for OpenGL BindTexture]
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	else {
		glUniform1i(shaderProgram.useTextureLocation, 0);  // do not sample the texture
	}
}
void setObjectUniforms(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float shininess) {
	glUniform3fv(boxShaderProgram.diffuseLocation, 1, glm::value_ptr(diffuse));  // 2nd parameter must be 1 - it declares number of vectors in the vector array
	glUniform3fv(boxShaderProgram.ambientLocation, 1, glm::value_ptr(ambient));
	glUniform3fv(boxShaderProgram.specularLocation, 1, glm::value_ptr(specular));
	glUniform1f(boxShaderProgram.shininessLocation, shininess);
}
void setLightUniforms(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, 
	const glm::vec3 &position, const float &constant, const float &linear, const float &quadratic, 
	const glm::vec3 &camDirection, const glm::vec3 &camPosition, const float &camCutoff, const float &camOuterCutoff) {
	glUniform3fv(boxShaderProgram.lightDiffuseLocation, 1, glm::value_ptr(diffuse));  // 2nd parameter must be 1 - it declares number of vectors in the vector array
	glUniform3fv(boxShaderProgram.lightAmbientLocation, 1, glm::value_ptr(ambient));
	glUniform3fv(boxShaderProgram.lightSpecularLocation, 1, glm::value_ptr(specular));
	//glUniform3fv(boxShaderProgram.lightDirectionLocation, 1, glm::value_ptr(direction));
	glUniform3fv(boxShaderProgram.lightPositionLocation, 1, glm::value_ptr(position));
	glUniform1f(boxShaderProgram.lightConstantPowerLocation, constant);
	glUniform1f(boxShaderProgram.lightLinearPowerLocation, linear);
	glUniform1f(boxShaderProgram.lightQuadraticPowerLocation, quadratic);

	glUniform3fv(boxShaderProgram.reflectorPositionLocation, 1, glm::value_ptr(camPosition));
	glUniform3fv(boxShaderProgram.reflectorDirectionLocation, 1, glm::value_ptr(camDirection));

	glUniform1f(boxShaderProgram.reflectorCutoffLocation, glm::cos(glm::radians(camCutoff)));
	glUniform1f(boxShaderProgram.reflectorOuterCutoffLocation, glm::cos(glm::radians(camOuterCutoff)));



}
void setColorModifierUniform(float colorModifier) {
	//std::cout << "Color modifier Uniform setter called\n";
	glUniform1f(shaderProgram.colorModifierLocation, colorModifier);
}
void setAlarmUniforms(std::list<AlarmObject> alarms/*AlarmObject alarm*/, bool playAlarms){
	///for (int i = 0; i < 1; i++) {
	//glm::vec3 dir = glm::vec3(1.0f, 1.0f, 0.0f);
	AlarmObject alarm = alarms.front();
		glUniform3fv(glGetUniformLocation(boxShaderProgram.program.programId, "alarms[0].position"),1, glm::value_ptr(alarm.position));
		glUniform3fv(glGetUniformLocation(boxShaderProgram.program.programId, "alarms[0].direction"), 1, glm::value_ptr(alarm.direction));
		glUniform3fv(glGetUniformLocation(boxShaderProgram.program.programId, "alarms[0].ambient"), 1, glm::value_ptr(alarm.lightAmbient));
		glUniform3fv(glGetUniformLocation(boxShaderProgram.program.programId, "alarms[0].diffuse"), 1, glm::value_ptr(alarm.lightDiffuse));
		glUniform3fv(glGetUniformLocation(boxShaderProgram.program.programId, "alarms[0].specular"), 1, glm::value_ptr(alarm.lightSpecular));
		glUniform1f(glGetUniformLocation(boxShaderProgram.program.programId, "alarms[0].cutOff"), alarm.lightCameraCutoff);
		glUniform1f(glGetUniformLocation(boxShaderProgram.program.programId, "alarms[0].outerCutoff"), alarm.lightCameraOuterCutoff);
		glUniform1f(glGetUniformLocation(boxShaderProgram.program.programId, "alarms[0].constantPower"), alarm.lightConstantPower);
		glUniform1f(glGetUniformLocation(boxShaderProgram.program.programId, "alarms[0].linearPower"), alarm.lightLinearPower);
		glUniform1f(glGetUniformLocation(boxShaderProgram.program.programId, "alarms[0].quadraticPower"), alarm.lightQuadraticPower);
		glUniform1i(glGetUniformLocation(boxShaderProgram.program.programId, "alarms[0].calculate"), playAlarms);
		//	boxShaderProgram.posLocation = glGetAttribLocation(boxShaderProgram.program.programId, "position"); //V

		//glUniform1f(boxShaderProgram.lightCameraCutoffLocation, glm::cos(glm::radians(camCutoff)));
	///}
}
void drawBox(BoxObject * box, const glm::mat4 & viewMatrix, 
	const glm::mat4 & projectionMatrix/*, GLuint texture*/, 
	SunObject * sun, Camera * camera, std::list<AlarmObject> alarms, AlarmObject* globalAlarm, bool playAlarms) {
	///glUseProgram(boxShaderProgram.program.programId);
	boxShaderProgram.program.use();

	//float angle = 0.5f;
	// prepare modelling transform matrix
	/*
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), box->position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(box->size));
	modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 0, 1));*/
	glm::mat4 modelMatrix = alignObject(box->position, box->direction, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(box->size));
	//
	///setTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);
	glm::mat4 PVM = projectionMatrix * viewMatrix * modelMatrix;
	glUniformMatrix4fv(boxShaderProgram.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));
	///setColorModifierUniform(0.99f);
	glUniform1f(boxShaderProgram.colorModifierLocation, 0.99f);
	setObjectUniforms(
		boxGeometry->ambient,
		boxGeometry->diffuse,
		boxGeometry->specular,
		boxGeometry->shininess
	);
	setLightUniforms(
		boxGeometry->lightAmbient,
		boxGeometry->lightDiffuse,
		boxGeometry->lightSpecular,
		sun->position,
		boxGeometry->lightConstantPower,
		boxGeometry->lightLinearPower,
		boxGeometry->lightQuadraticPower,

		camera->direction,
		camera->position,
		boxGeometry->lightCameraCutoff,
		boxGeometry->lightCameraOuterCutoff
	);
	//FlashLight
	glUniform3fv(glGetUniformLocation(boxShaderProgram.program.programId, "reflector.ambient"), 1, glm::value_ptr(glm::vec3(0.2f, 0.5f, 0.5f)));
	glUniform3fv(glGetUniformLocation(boxShaderProgram.program.programId, "reflector.diffuse"), 1, glm::value_ptr(glm::vec3(0.9f, 0.9f, 0.9f)));
	glUniform3fv(glGetUniformLocation(boxShaderProgram.program.programId, "reflector.specular"), 1, glm::value_ptr(glm::vec3(0.1f, 0.1f, 0.1f)));
	glUniform1f(glGetUniformLocation(boxShaderProgram.program.programId, "reflector.constantPower"), 1.0f);
	glUniform1f(glGetUniformLocation(boxShaderProgram.program.programId, "reflector.linearPower"), 0.09f);
	glUniform1f(glGetUniformLocation(boxShaderProgram.program.programId, "reflector.quadraticPower"), 0.032f);


	setAlarmUniforms(alarms/*globalAlarm*/, playAlarms);
	glUniform1i(boxShaderProgram.texSamplerLocation, 0);
	glUniform3fv(boxShaderProgram.lightSourceColorLocation, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	glUniform3fv(boxShaderProgram.ambientLocation, 1, glm::value_ptr(boxGeometry->ambient));
	glUniform3fv(boxShaderProgram.sunLocation, 1, glm::value_ptr(sun->position));
	glUniform3fv(boxShaderProgram.viewerLocation, 1, glm::value_ptr(camera->position));

	glUniformMatrix4fv(boxShaderProgram.VmatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(boxShaderProgram.MmatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));
	glUniformMatrix4fv(boxShaderProgram.normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	///setAttributesUniforms(texture);
	GLsizei vertices_count = 36/*sizeof(boxVertices)/sizeof(*boxVertices)*/;

	///<Textura 1>
	glActiveTexture(GL_TEXTURE0 + 0); ///Offset texturovací jednotky (0-15)
	glBindTexture(GL_TEXTURE_2D, boxGeometry->texture);
	///<Textura 2>	
	  // ......

	glBindVertexArray(boxGeometry->vertexArrayObject);
	CHECK_GL_ERROR();
	//glDrawElements(GL_TRIANGLES, boxGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, vertices_count);
	CHECK_GL_ERROR();

	glBindVertexArray(0);
	glUseProgram(0);

	return;
}
void drawSkybox(const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {
	/*glDepthFunc(GL_LEQUAL);
	skyboxShaderProgram.program->use();
	glm::mat4 View = glm::mat4(glm::mat3(viewMatrix)); // remove translation

	glUniformMatrix4fv(skyboxShaderProgram.PmatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(skyboxShaderProgram.VmatrixLocation, 1, GL_FALSE, glm::value_ptr(View));
	///glUniform1i(/*glGetUniformLocation(skyboxShaderProgram.program->programId, "skybox")*//*skyboxShaderProgram.texSamplerLocation, 0);
	glUniform1i(skyboxShaderProgram.texSamplerLocation, 0);
	CHECK_GL_ERROR();

	GLsizei vertices_count = 36;
	glBindVertexArray(skyboxGeometry->vertexArrayObject);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxGeometry->texture);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices_count);
	CHECK_GL_ERROR();

	glBindVertexArray(0);
	glUseProgram(0);
	glDepthFunc(GL_LESS);*/
	skyboxShaderProgram.program->use();
	glm::mat4 matrix = projectionMatrix * viewMatrix;

	glm::mat4 viewRotation = viewMatrix;
	viewRotation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	glm::mat4 inversePVmatrix = glm::inverse(projectionMatrix * viewRotation);

	glUniformMatrix4fv(skyboxShaderProgram.PVmatrixLocation, 1, GL_FALSE, glm::value_ptr(inversePVmatrix));
	glUniform1i(skyboxShaderProgram.texSamplerLocation, 0);
	glBindVertexArray(skyboxGeometry->vertexArrayObject);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxGeometry->texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, skyboxGeometry->numTriangles + 2);
	glBindVertexArray(0);
	glUseProgram(0);
}
void drawSquare(SquareObject * square, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix, float colorModifier) {
	//glUseProgram(shaderProgram.program);
	squareProgram->use();
	//float angle = 0.5f;
	// prepare modelling transform matrix
	/*
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), box->position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(box->size));
	modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 0, 1));*/

	glm::mat4 modelMatrix = alignObject(square->position, square->direction, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(square->size));
	//
	setTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);
	setColorModifierUniform(colorModifier);
	GLsizei vertices_count = 6/*sizeof(boxVertices)/sizeof(*boxVertices)*/;

	glActiveTexture(GL_TEXTURE0 + 0); ///Offset texturovací jednotky (0-15)
	glBindTexture(GL_TEXTURE_2D, squareGeometry->texture);
	glBindVertexArray(squareGeometry->vertexArrayObject);
	//glDrawElements(GL_TRIANGLES, boxGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, vertices_count);
	CHECK_GL_ERROR();

	glBindVertexArray(0);
	glUseProgram(0);

	return;
}
void drawSun(SunObject * sun, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix) {
	sunShaderProgram.program.use();
	glm::mat4 Model = glm::mat4(1.0f);
	//Model = glm::translate(Model, glm::vec3(0.0f, -1.75f, 0.0f));
	Model = glm::scale(Model, glm::vec3(1.2f, 1.2f, 1.2f));
	glm::mat4 modelMatrix = alignObject(sun->position, sun->direction, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix/*Model*/, glm::vec3(sun->size));

	setTransformUniforms(modelMatrix, viewMatrix, projectionMatrix);
	glUniform3fv(sunShaderProgram.lightSourceColorLocation, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	glUniform3fv(sunShaderProgram.colorLocation, 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.0f)));

	GLsizei vertices_count = 12;
	glBindVertexArray(sunGeometry->vertexArrayObject);
	//glDrawElements(GL_TRIANGLES, boxGeometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, vertices_count);
	CHECK_GL_ERROR();

	glBindVertexArray(0);
	
	glUseProgram(0);

	return;
}
void drawAlarm(AlarmObject * alarm, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix, Camera * camera) {
	alarmShaderProgram.program.use();

	glm::mat4 modelMatrix = alignObject(alarm->position, alarm->direction, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(alarm->size));
	setTransformUniforms(modelMatrix, viewMatrix, projectionMatrix); //bind na aktualni program

	glUniform3fv(alarmShaderProgram.colorLocation, 1, glm::value_ptr(alarm->color));

	GLsizei vertices_count = 12;
	glBindVertexArray(alarmGeometry->vertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, vertices_count);
	CHECK_GL_ERROR();

	glBindVertexArray(0);

	glUseProgram(0);

	return;
}

void drawAssimpModel(Model * model, Camera * camera, std::list<AlarmObject> alarms, bool playAlarms){
	modelProgram2->use();
	///glm::mat4 Projection = glm::perspective(glm::radians(25.0f), (float)SCREEN_HEIGHT / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(modelProgram2->programId, "projection"), 1, GL_FALSE, glm::value_ptr(camera->Projection));
	CHECK_GL_ERROR();
	glUniformMatrix4fv(glGetUniformLocation(modelProgram2->programId, "view"), 1, GL_FALSE, glm::value_ptr(camera->View));
	CHECK_GL_ERROR();
	setAlarmUniforms(alarms/*globalAlarm*/, playAlarms);
	CHECK_GL_ERROR();
	glm::mat4 Model = glm::mat4(1.0f);
	//Model = glm::translate(Model, glm::vec3(0.0f, -1.75f, 0.0f));
	Model = glm::scale(Model, glm::vec3(0.05f, 0.05f, 0.05f));
	glm::mat4 modelMatrix = alignObject(glm::vec3(-2.6f, 6.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
	glUniformMatrix4fv(glGetUniformLocation(modelProgram2->programId, "model"), 1, GL_FALSE, glm::value_ptr(/*Model*/ modelMatrix));
	model->DrawModel(modelProgram2);
	CHECK_GL_ERROR();
	return;
}

void cleanupShaderPrograms(void) {
	pgr::deleteProgramAndShaders(shaderProgram.program);
	boxProgram->deleteShaders();
	delete boxProgram;
	sunProgram->deleteShaders();
	delete sunProgram;
}

void initializeShaderPrograms(void) {
	std::cout << "Init Shaders called\n";
	std::vector<GLuint> shaderList;
	// push vertex shader and fragment shader
	shaderList.push_back(pgr::createShaderFromSource(GL_VERTEX_SHADER, colorVertexShaderSrc));
	shaderList.push_back(pgr::createShaderFromSource(GL_FRAGMENT_SHADER, colorFragmentShaderSrc));	
	///shaderList.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/boxShader.vert"));
	///shaderList.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/boxShader.frag"));
	//| == Shader program transormace
	shaderProgram.program = pgr::createProgram(shaderList);
	///Lokace shader inputu
	shaderProgram.posLocation = glGetAttribLocation(shaderProgram.program, "position");
	shaderProgram.colorLocation = glGetAttribLocation(shaderProgram.program, "color");
	shaderProgram.texCoordLocation = glGetAttribLocation(shaderProgram.program, "texCoord");
	// get uniforms locations
	shaderProgram.colorModifierLocation = glGetUniformLocation(shaderProgram.program, "cm");
	shaderProgram.PVMmatrixLocation = glGetUniformLocation(shaderProgram.program, "PVMmatrix");
	shaderList.clear();
	//\\| /== konec shader programu
	std::cout << "Init Shaders ended\n";

	//BOXES
	///Program * boxProgram = new Program(colorVertexShaderSrc, colorFragmentShaderSrc);
	const char * VS = "shaders/boxShader.vert";
	const char * FS = "shaders/boxShader.frag";
	//Program * boxProgram = new Program(boxVertexShaderSrc, boxFragmentShaderSrc);
	/*---*/boxProgram = new Program(VS, FS, "BOX");
	boxShaderProgram.program = *boxProgram;
	// lokace vstupnich atributu shaderu
	boxShaderProgram.posLocation = glGetAttribLocation(boxShaderProgram.program.programId, "position"); //V
	boxShaderProgram.colorLocation = glGetAttribLocation(boxShaderProgram.program.programId, "color"); //V
	boxShaderProgram.texCoordLocation = glGetAttribLocation(boxShaderProgram.program.programId, "texCoord"); //V
	boxShaderProgram.normalLocation = glGetAttribLocation(boxShaderProgram.program.programId, "normal");
	// lokace vstupnich uniformu shaderu
	boxShaderProgram.colorModifierLocation = glGetUniformLocation(boxShaderProgram.program.programId, "cm"); //V
	boxShaderProgram.PVMmatrixLocation = glGetUniformLocation(boxShaderProgram.program.programId, "PVMmatrix");
	boxShaderProgram.VmatrixLocation = glGetUniformLocation(boxShaderProgram.program.programId, "VMatrix"); //V
	boxShaderProgram.MmatrixLocation = glGetUniformLocation(boxShaderProgram.program.programId, "MMatrix"); //V
	boxShaderProgram.normalMatrixLocation = glGetUniformLocation(boxShaderProgram.program.programId, "normalMatrix"); //V

	//opticke vlastnosti objektu
	boxShaderProgram.ambientLocation = glGetUniformLocation(boxShaderProgram.program.programId, "material.ambient"); //V
	boxShaderProgram.diffuseLocation = glGetUniformLocation(boxShaderProgram.program.programId, "material.diffuse"); //V
	boxShaderProgram.specularLocation = glGetUniformLocation(boxShaderProgram.program.programId, "material.specular"); //V
	boxShaderProgram.shininessLocation = glGetUniformLocation(boxShaderProgram.program.programId, "material.shininess");  //V

	//opticke vlastnosti svetelneho zdroje
	boxShaderProgram.lightAmbientLocation = glGetUniformLocation(boxShaderProgram.program.programId, "pointLight.ambient"); //V
	boxShaderProgram.lightDiffuseLocation = glGetUniformLocation(boxShaderProgram.program.programId, "pointLight.diffuse"); //V
	boxShaderProgram.lightSpecularLocation = glGetUniformLocation(boxShaderProgram.program.programId, "pointLight.specular"); //V
	boxShaderProgram.lightPositionLocation = glGetUniformLocation(boxShaderProgram.program.programId, "pointLight.position"); //V
	boxShaderProgram.lightConstantPowerLocation = glGetUniformLocation(boxShaderProgram.program.programId, "pointLight.constantPower"); //V
	boxShaderProgram.lightLinearPowerLocation = glGetUniformLocation(boxShaderProgram.program.programId, "pointLight.linearPower"); //V
	boxShaderProgram.lightQuadraticPowerLocation = glGetUniformLocation(boxShaderProgram.program.programId, "pointLight.quadraticPower"); //V

	//1st person reflector
	boxShaderProgram.reflectorPositionLocation = glGetUniformLocation(boxShaderProgram.program.programId, "reflector.position"); //V
	boxShaderProgram.reflectorDirectionLocation = glGetUniformLocation(boxShaderProgram.program.programId, "reflector.direction"); //V
	boxShaderProgram.reflectorCutoffLocation = glGetUniformLocation(boxShaderProgram.program.programId, "reflector.cutOff"); //V4
	boxShaderProgram.reflectorOuterCutoffLocation = glGetUniformLocation(boxShaderProgram.program.programId, "reflector.outerCutoff"); //V4

	boxShaderProgram.sunLocation = glGetUniformLocation(boxShaderProgram.program.programId, "sunLocation");  //V
	boxShaderProgram.viewerLocation = glGetUniformLocation(boxShaderProgram.program.programId, "viewerLocation"); 

	boxShaderProgram.texSamplerLocation = glGetUniformLocation(boxShaderProgram.program.programId, "ourTexture"); //texturovaci jednotka OpenGL
	boxShaderProgram.lightSourceColorLocation = glGetUniformLocation(boxShaderProgram.program.programId, "lightSourceColor");
	boxShaderProgram.ambientLocation = glGetUniformLocation(boxShaderProgram.program.programId, "ambientLighting");

	boxProgram->deleteShaders();

	//LIGHT
	VS = "shaders/lightShader.vert";
	FS = "shaders/lightShader.frag";
	sunProgram = new Program(VS, FS, "LIGHT");
	sunShaderProgram.program = *sunProgram;

	sunShaderProgram.posLocation = glGetAttribLocation(sunShaderProgram.program.programId, "position");
	///sunShaderProgram.texCoordLocation = glGetAttribLocation(boxShaderProgram.program.programId, "texCoord"); Not used yet
	// get uniforms locations
	sunShaderProgram.colorLocation = glGetUniformLocation(sunShaderProgram.program.programId, "color");
	sunShaderProgram.lightSourceColorLocation = glGetUniformLocation(sunShaderProgram.program.programId, "lightSourceColor");
	sunShaderProgram.PVMmatrixLocation = glGetUniformLocation(sunShaderProgram.program.programId, "PVMmatrix");
	///sunShaderProgram.texSamplerLocation = glGetUniformLocation(boxShaderProgram.program.programId, "ourTexture");Not used yed

	sunProgram->deleteShaders();

	//ALARM
	VS = "shaders/alarmShader.vert";
	FS = "shaders/alarmShader.frag";
	alarmProgram = new Program(VS, FS, "ALARM");
	alarmShaderProgram.program = *alarmProgram;

	alarmShaderProgram.posLocation = glGetAttribLocation(alarmShaderProgram.program.programId, "position");
	///sunShaderProgram.texCoordLocation = glGetAttribLocation(boxShaderProgram.program.programId, "texCoord"); Not used yet
	// get uniforms locations
	alarmShaderProgram.colorLocation = glGetUniformLocation(alarmShaderProgram.program.programId, "color");
	alarmShaderProgram.PVMmatrixLocation = glGetUniformLocation(alarmShaderProgram.program.programId, "PVMmatrix");

	alarmProgram->deleteShaders();

	//MODEL
	VS = "shaders/modelShader.vert";
	FS = "shaders/modelShader.frag";
	modelProgram2 = new Program(VS, FS, "MODEL-render");
	modelProgram2->deleteShaders();

	//SKYBOX
	VS = "shaders/skyboxShader.vert";
	FS = "shaders/skyboxShader.frag";
	skyboxProgram = new Program(VS, FS, "SKYBOX");
	skyboxShaderProgram.program = skyboxProgram;
	/*
	skyboxShaderProgram.posLocation = glGetAttribLocation(skyboxShaderProgram.program->programId, "position"); 

	skyboxShaderProgram.PmatrixLocation = glGetUniformLocation(skyboxShaderProgram.program->programId, "Pmatrix");
	skyboxShaderProgram.VmatrixLocation = glGetUniformLocation(skyboxShaderProgram.program->programId, "Vmatrix");
	skyboxShaderProgram.texSamplerLocation = glGetUniformLocation(skyboxShaderProgram.program->programId, "skybox");*/
	skyboxShaderProgram.screenCoordLocation = glGetAttribLocation(skyboxShaderProgram.program->programId, "screenCoord");

	skyboxShaderProgram.texSamplerLocation = glGetUniformLocation(skyboxShaderProgram.program->programId, "skybox");
	skyboxShaderProgram.PVmatrixLocation = glGetUniformLocation(skyboxShaderProgram.program->programId, "invPVmatrix");
	
	skyboxShaderProgram.program->deleteShaders();

	//SQUARE
	VS = "shaders/squareShader.vert";
	FS = "shaders/squareShader.frag";
	squareProgram = new Program(VS, FS, "SQUARE");
	squareProgram->deleteShaders();
}

///
///Load single mesh metoda --------
///

void initSkyboxGeometry(GLuint shader, MeshGeometry **geometry) {
	*geometry = new MeshGeometry;
	static const float screenCoords[] = {
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		-1.0f,  1.0f,
		 1.0f,  1.0f
	};
	glGenVertexArrays(1, &((*geometry)->vertexArrayObject)); //generovani vao
	glBindVertexArray((*geometry)->vertexArrayObject); //bind vao

	glGenBuffers(1, &((*geometry)->vertexBufferObject)); //generovani vbo
	glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject); //bind vbo

	///glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxObjectVertices), skyboxObjectVertices, GL_STATIC_DRAW); //naplneni vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenCoords), screenCoords, GL_STATIC_DRAW);
	CHECK_GL_ERROR();

	///glEnableVertexAttribArray(/*skyboxShaderProgram.posLocation*/0);
	///glVertexAttribPointer(/*skyboxShaderProgram.posLocation*/0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(skyboxShaderProgram.screenCoordLocation);
	glVertexAttribPointer(skyboxShaderProgram.screenCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

	std::vector<std::string> faces = {
		"data/textures/skybox_nebula/purplenebula_bk.tga",
		"data/textures/skybox_nebula/purplenebula_dn.tga",
		"data/textures/skybox_nebula/purplenebula_ft.tga",
		"data/textures/skybox_nebula/purplenebula_lf.tga",
		"data/textures/skybox_nebula/purplenebula_rt.tga",
		"data/textures/skybox_nebula/purplenebula_up.tga"
	};
	std::vector<std::string> faces2 = {
		"data/textures/skybox/right.jpg",
		"data/textures/skybox/left.jpg",
		"data/textures/skybox/top.jpg",/*"data/textures/skybox/right.jpg",*/
		"data/textures/skybox/bottom.jpg",/*"data/textures/skybox/left.jpg",*/
		"data/textures/skybox/front.jpg",/*"data/textures/skybox/top.jpg",//top*/
		"data/textures/skybox/back.jpg"/*"data/textures/skybox/bottom.jpg"*/
	};
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &((*geometry)->texture));
	glBindTexture(GL_TEXTURE_CUBE_MAP, (*geometry)->texture);

	CHECK_GL_ERROR();
	int width, height, channels;

	for (GLuint i = 0; i < faces.size(); i++) {
		unsigned char * data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
		CHECK_GL_ERROR();
		if (data) {   // Iterace skrz enum stran cube mapy
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			std::cout << "[RENDER] Texture successfuly loaded\n";
		}
		else {
			std::cout << "[ERROR][RENDER] Failed to load actual texture\n";
			stbi_image_free(data); //uvolnìní pamìti
		}

	}
	(*geometry)->numTriangles = 2;
	CHECK_GL_ERROR();

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindVertexArray(0);
}
void initBoxGeometry(GLuint shader, MeshGeometry **geometry) {
	*geometry = new MeshGeometry;

	(*geometry)->ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	(*geometry)->diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	(*geometry)->specular = glm::vec3(0.5f, 0.5f, 0.5f);
	(*geometry)->shininess = 2.0f;

	//Hardcoded moving light
	(*geometry)->lightAmbient = glm::vec3(0.2f, 0.5f, 0.5f)/*(0.2f, 0.02f, 0.02f)*/;
	(*geometry)->lightDiffuse = glm::vec3(0.9f, 0.9f, 0.9f)/*(0.9f, 0.0f, 0.0f)*/;
	(*geometry)->lightSpecular = glm::vec3(0.1f, 0.1f, 0.1f)/*(0.2f, 0.0f, 0.0f)*/;

	(*geometry)->lightConstantPower = 1.0f;
	(*geometry)->lightLinearPower = 0.09f;
	(*geometry)->lightQuadraticPower = 0.032f;

	(*geometry)->lightCameraCutoff = 12.5f;
	(*geometry)->lightCameraOuterCutoff = 20.5f;

	glGenVertexArrays(1, &((*geometry)->vertexArrayObject)); //generovani vao
	glBindVertexArray((*geometry)->vertexArrayObject); //bind vao

	glGenBuffers(1, &((*geometry)->vertexBufferObject)); //generovani vbo
	glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject); //bind vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW); //naplneni vbo
	CHECK_GL_ERROR();


	glEnableVertexAttribArray(boxShaderProgram.posLocation);
	// vertices of triangles - start at the beginning of the array
	glVertexAttribPointer(boxShaderProgram.posLocation, 3, GL_FLOAT, GL_FALSE,/* 3 * 3 * sizeof(float)*/ ((3 * 3) + 2) * sizeof(float), 0); //3 slozky - xyz, stride 3*2*size
	glEnableVertexAttribArray(boxShaderProgram.colorLocation);
	glVertexAttribPointer(boxShaderProgram.colorLocation, 3, GL_FLOAT, GL_FALSE,/* 9 * sizeof(float)*/ ((3 * 3) + 2) * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(boxShaderProgram.normalLocation);
	glVertexAttribPointer(boxShaderProgram.normalLocation, 3, GL_FLOAT, GL_FALSE,/* 9 * sizeof(float)*/ ((3 * 3) + 2) * sizeof(float), (void*)(8 * sizeof(float)));

	///============ <<Textures>> ====================
	glGenTextures(1, &((*geometry)->texture));
	glBindTexture(GL_TEXTURE_2D, (*geometry)->texture);
	CHECK_GL_ERROR();

	///===<nastavení vlastností aktuálního texturovacího objektu>====
	  ///=== < 1) TEXTURE WRAPPING > ===
		// = wrapping - pro souøadnice [|x|, |y|] > [1, 1]		   |
		//   - 1] Target (s jakým typem textury pracujeme)         |  
		//   - 2] Urèení osy, ve které má být pravidlo aplikováno  |	
		//	 - 3] Nastavení vlastnosti						       |   
		// V pøípadì nastavení GL_CLAMP_TO_BORDER je vhodné       \ / 
		// také nastavit vlastnoti okraje GL_TEXTURE_BORDER_COLOR  V
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //nastavení pro osu S (=x) souøadnic textury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  // -||- pro osu T (=y)	
	CHECK_GL_ERROR();
	  ///=== < 2) TEXTURE FILTERING > ===
		// = mapování texturovovacích souøadnic na texely (texture pixely) => výpoèet souøadnich / kombinace barev sousedních texelù
		//   - GL_NEAREST - nejbližší pixel textury k dané souøadnici (ostré, rozpixelované)				 |
		//   - GL_LINEAR - (bilinear filtering) - interpolace hodnot (rozmazané, nerozpixelované)			 |
		//		barev sousedních texelù v závislosti na vzdálenosti											 /
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //minifying pro downscaling	   /____/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  //magnifying pro upscaling      \

	CHECK_GL_ERROR();
	int width, height, channels;
	//unsigned char * data = stbi_load("data/textures/grassBlock.png", &width, &height, &channels, 0);
	unsigned char * data = stbi_load("data/textures/scifiBlock.png", &width, &height, &channels, 0);
	CHECK_GL_ERROR();
	if (data) {
		//   - Target(ovlivni jen daný texturovaci objekt na stejném targetu | 
		//   - Mipmap level, 0 default										 |
		//	 - Formát uložení textury (RGB, RGBA..)							 |
		//	 - width + height = rozmìry výsledné textury					 |
		//	 - 0 - vždy 0, nepodstatné									     | 
		//	 - formát (RGB..) a datový typ zdrojového obrázku				\ /
		//	 - data obrázku													 V
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		///nyní má daný texturovací objekt na sebe navázaný obrázek textury, zatím pouze základní level textury
		glGenerateMipmap(GL_TEXTURE_2D); ///automatické dogenerování dalších vrstev (mipmap)
		std::cout << "[RENDER] Texture successfuly loaded\n";
	}
	else {
		std::cout << "[ERROR][RENDER] Failed to load actual texture\n";
	}
	stbi_image_free(data); //uvolnìní pamìti

	glEnableVertexAttribArray(boxShaderProgram.texCoordLocation);
	glVertexAttribPointer(boxShaderProgram.texCoordLocation, /*3*/2, GL_FLOAT, GL_FALSE, ((3 * 3) + 2) * sizeof(float), (void*)(6 *sizeof(float)));

	glBindVertexArray(0);
}
void initSquareGeometry(SCommonShaderProgram &shader, MeshGeometry **geometry) {
	std::cout << "Square Geometry init\n";
	*geometry = new MeshGeometry;

	glGenVertexArrays(1, &((*geometry)->vertexArrayObject)); //generovani vao
	glBindVertexArray((*geometry)->vertexArrayObject); //bind vao
	CHECK_GL_ERROR();
	glGenBuffers(1, &((*geometry)->vertexBufferObject)); //generovani vbo
	glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject); //bind vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(flatSurfaceElementVertices), flatSurfaceElementVertices, GL_STATIC_DRAW); //naplneni vbo
	CHECK_GL_ERROR();

	GLint posLoc = glGetAttribLocation(squareProgram->programId, "position");
	glEnableVertexAttribArray(/*shader.posLocation*/ posLoc);
	// vertices of triangles - start at the beginning of the array
	glVertexAttribPointer(/*shader.posLocation*/ posLoc, 3, GL_FLOAT, GL_FALSE, ((3 * 3) + 2) * sizeof(float), 0); //3 slozky - xyz, stride 3*2*size
	GLint colLoc = glGetAttribLocation(squareProgram->programId, "color");
	glEnableVertexAttribArray(/*shader.colorLocation*/colLoc);
	glVertexAttribPointer(/*shader.colorLocation*/colLoc, 3, GL_FLOAT, GL_FALSE, ((3 * 3) + 2) * sizeof(float), (void*)(3 * sizeof(float)));

	///============ <<Textures>> ====================
	glGenTextures(1, &((*geometry)->texture));
	glBindTexture(GL_TEXTURE_2D, (*geometry)->texture);
	CHECK_GL_ERROR();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	CHECK_GL_ERROR();
									 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  

	CHECK_GL_ERROR();
	int width, height, channels;
	unsigned char * data = stbi_load("data/textures/moonBlock2.png", &width, &height, &channels, 0);
	CHECK_GL_ERROR();
	if (data) {												
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		///nyní má daný texturovací objekt na sebe navázaný obrázek textury, zatím pouze základní level textury
		glGenerateMipmap(GL_TEXTURE_2D); ///automatické dogenerování dalších vrstev (mipmap)
		std::cout << "[RENDER] Texture successfuly loaded\n";
	}
	else {
		std::cout << "[ERROR][RENDER] Failed to load actual texture\n";
	}
	stbi_image_free(data); //uvolnìní pamìti

	GLint texLoc = glGetAttribLocation(squareProgram->programId, "texCoord");
	glEnableVertexAttribArray(texLoc);
	glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, ((3 * 3) + 2) * sizeof(float), (void*)(6 * sizeof(float)));

	glBindVertexArray(0);
}
void initSunGeometry(GLuint shader, MeshGeometry **geometry) {
	*geometry = new MeshGeometry;
	glGenVertexArrays(1, &((*geometry)->vertexArrayObject)); //generovani vao
	glBindVertexArray((*geometry)->vertexArrayObject); //bind vao

	glGenBuffers(1, &((*geometry)->vertexBufferObject)); //generovani vbo
	glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject); //bind vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(sunObjectVertices), sunObjectVertices, GL_STATIC_DRAW); //naplneni vbo
	CHECK_GL_ERROR();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindVertexArray(0);
}

void initAlarmGeometry(GLuint shader, MeshGeometry **geometry) {
	*geometry = new MeshGeometry;
	glGenVertexArrays(1, &((*geometry)->vertexArrayObject)); //generovani vao
	glBindVertexArray((*geometry)->vertexArrayObject); //bind vao

	glGenBuffers(1, &((*geometry)->vertexBufferObject)); //generovani vbo
	glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject); //bind vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(alarmObjectVertices), alarmObjectVertices, GL_STATIC_DRAW); //naplneni vbo
	CHECK_GL_ERROR();

	glEnableVertexAttribArray(alarmShaderProgram.posLocation);
	glVertexAttribPointer(alarmShaderProgram.posLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindVertexArray(0);
}

void initializeModels() {
	///initBoxGeometry(shaderProgram, &boxGeometry);
	initSquareGeometry(shaderProgram, &squareGeometry);
	initBoxGeometry(boxShaderProgram.program.programId, &boxGeometry);
	initSunGeometry(sunShaderProgram.program.programId, &sunGeometry);
	initAlarmGeometry(alarmShaderProgram.program.programId, &alarmGeometry);
	initSkyboxGeometry(skyboxShaderProgram.program->programId, &skyboxGeometry);
}
void cleanupGeometry(MeshGeometry *geometry) {
	glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
	glDeleteBuffers(1, &(geometry->elementBufferObject));
	glDeleteBuffers(1, &(geometry->vertexBufferObject));
}
void cleanupModels() {
	cleanupGeometry(boxGeometry);
	cleanupGeometry(squareGeometry);
	cleanupGeometry(sunGeometry);
	cleanupGeometry(alarmGeometry);
}