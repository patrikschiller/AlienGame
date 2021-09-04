//----------------------------------------------------------------------------------------
/**
 * \file       CosmoSuit.cpp
 * \author     Patrik Schiller
 * \date       2019/21/05
 *
*/
//----------------------------------------------------------------------------------------
#include "pch.h"
#include "CosmoSuit.h"
#include "spline.h"
#include "Reflector.h"
#include "PointLight.h"
#include "DirectionalLight.h"



CosmoSuit::~CosmoSuit()
{
}

void CosmoSuit::Draw(Program * program, Camera * camera, Reflector * reflector, GameObjectsList pointlights, GameObjectsList dirLights, GameObjectsList alarms, bool playAlarms)
{
	program->use();
	reflector->setUniforms(program, 0);

	GLuint index = 0;
	for (/*std::vector<PointLight>::iterator*/GameObjectsList::iterator it = pointlights.begin(); it != pointlights.end(); ++it) {
		CHECK_GL_ERROR();
		PointLight * PL = (PointLight*)(*it);
		PL->setUniforms(program, index);
		index++;
	}
	index = 0;
	for (/*std::vector<PointLight>::iterator*/GameObjectsList::iterator it = dirLights.begin(); it != dirLights.end(); ++it) {
		CHECK_GL_ERROR();
		DirectionalLight * DL = (DirectionalLight*)(*it);
		DL->setUniforms(program, index);
		index++;
	}
	if (playAlarms) {
		index = 0;
		for (GameObjectsList::iterator it = alarms.begin(); it != alarms.end(); ++it) {
			//std::cout << "setting alarm uniform " << index << "\n";
			CHECK_GL_ERROR();
			Reflector * RF = (Reflector*)(*it);
			RF->setAlarmUniforms(program, index);
			index++;
		}
	}


	CHECK_GL_ERROR();

	glUniformMatrix4fv(glGetUniformLocation(program->programId, "projection"), 1, GL_FALSE, glm::value_ptr(camera->Projection));
	CHECK_GL_ERROR();
	glUniformMatrix4fv(glGetUniformLocation(program->programId, "view"), 1, GL_FALSE, glm::value_ptr(camera->View));
	CHECK_GL_ERROR();

	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 modelMatrix = alignObject(
		this->geometry.position,
		this->geometry.direction,
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(geometry.size));
	glUniformMatrix4fv(glGetUniformLocation(program->programId, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	CHECK_GL_ERROR();
	glUniform4fv(glGetUniformLocation(program->programId, "color"), 1, glm::value_ptr(glm::vec4(geometry.color, 1.0f)));
	CHECK_GL_ERROR();
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(program->programId, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
	CHECK_GL_ERROR();
	glUniform3fv(glGetUniformLocation(program->programId, "viewerPos"), 1, glm::value_ptr(camera->position));
	CHECK_GL_ERROR();


	this->model->DrawModel(program);
	CHECK_GL_ERROR();
	glUseProgram(0);
	return;
}

bool CosmoSuit::initShaderProgram()
{
	const char * VS = "shaders/modelShader.vert";
	const char * FS = "shaders/modelShader.frag";
	program = new Program(VS, FS, "MODEL-render");
	program->deleteShaders();
	if (program) {
		std::cout << "[Cosmosuit] shaders initialized \n";
		return true;
	}
	std::cout << "[Cosmosuit] ERROR: shaders weren't initialized \n";
	return false;
}

glm::vec3 CosmoSuit::Move::evaluateClosedCurve(const glm::vec3 points[], const size_t count, const float t)
{
	glm::vec3 result(0.0, 0.0, 0.0);

	// based on the value of parametr t first find corresponding segment and its control points => i
	// and then call evaluateCurveSegment function with proper parameters to get a point on a closed curve

	float param = this->cyclic_clamp(t, 0.0f, float(count));
	size_t index = size_t(param);

	result = this->evaluateCurveSegment(
		points[(index - 1 + count) % count],
		points[(index) % count],
		points[(index + 1) % count],
		points[(index + 2) % count],
		param - floor(param)
	);

	return result;
}

glm::vec3 CosmoSuit::Move::evaluateClosedCurve_1stDerivative(const glm::vec3 points[], const size_t count, const float t)
{
	glm::vec3 result(1.0, 0.0, 0.0);

	// based on the value of parametr t first find corresponding curve segment and its control points => i
	// and then call evaluateCurveSegment_1stDerivative function with proper parameters
	// to get a derivative for the given point on a closed curve

	float param = this->cyclic_clamp(t, 0.0f, float(count));
	size_t index = size_t(param);

	result = this->evaluateCurveSegment_1stDerivative(
		points[(index - 1 + count) % count],
		points[(index) % count],
		points[(index + 1) % count],
		points[(index + 2) % count],
		param - floor(param)
	);

	return result;
}
///Private
glm::vec3 CosmoSuit::Move::evaluateCurveSegment(const glm::vec3 & P0, const glm::vec3 & P1, const glm::vec3 & P2, const glm::vec3 & P3, const float t)
{
	glm::vec3 result(0.0, 0.0, 0.0);

	// evaluate point on a curve segment (defined by the control points P0...P3)
	// for the given value of parametr t

	const float t2 = t * t;
	const float t3 = t * t2;

	result = P0 * (-t3 + 2.0f*t2 - t)
		+ P1 * (3.0f*t3 - 5.0f*t2 + 2.0f)
		+ P2 * (-3.0f*t3 + 4.0f*t2 + t)
		+ P3 * (t3 - t2);

	result *= 0.5f;

	return result;
}

glm::vec3 CosmoSuit::Move::evaluateCurveSegment_1stDerivative(const glm::vec3 & P0, const glm::vec3 & P1, const glm::vec3 & P2, const glm::vec3 & P3, const float t)
{
	glm::vec3 result(1.0, 0.0, 0.0);

	// evaluate first derivative for a point on a curve segment (defined by the control points P0...P3)
	// for the given value of parametr t

	const float t2 = t * t;

	result = P0 * (-3.0f*t2 + 4.0f*t - 1.0f)
		+ P1 * (9.0f*t2 - 10.0f*t)
		+ P2 * (-9.0f*t2 + 8.0f*t + 1.0f)
		+ P3 * (3.0f*t2 - 2.0f*t);

	result *= 0.5f;

	return result;
}
