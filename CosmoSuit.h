//----------------------------------------------------------------------------------------
/**
 * \file       CosmoSuit.h
 * \author     Patrik Schiller
 * \date       2019/21/05
 *
*/
//----------------------------------------------------------------------------------------
#pragma once
#include "Model.h"
#include "Camera.h"
#include "Reflector.h"
#include <list>
class CosmoSuit
{
public:
	typedef std::list<void *> GameObjectsList;
	Model * model;
	Program * program;
	GLint id;
	/**
	Structure that represents geometry of given Light
	*/
	struct Geometry {
		glm::vec3 position;
		glm::vec3 initPosition;
		glm::vec3 direction;
		glm::vec3 color;
		float size;
		//std::string modelPath;
	}geometry;
	/**
	Structure that represents move geometry and function of given Light
	*/
	struct Move {
		float isMoving = false;
		float speed;
		float animationStart;
		float currentTime;

		/**
			Initializes motion of current model
			@param speed - motion speed
			@param elapsedTimeStart - motion start time
		*/
		void initMotion(float speed, float elapsedTimeStart) {
			this->speed = speed;
			this->isMoving = true;
			this->animationStart = elapsedTimeStart;
			this->currentTime = elapsedTimeStart;
		}

		/**
			Stops motion of current model
		*/
		void stopMotion() {
			this->isMoving = true;
		}

		/**
			Calculates new position on Catmul-Rom spline
		*/
		glm::vec3 evaluateClosedCurve(
			const glm::vec3 points[],
			const size_t    count,
			const float     t
		);
		/**
			Calculates new direction on Catmul-Rom spline
		*/
		glm::vec3 evaluateClosedCurve_1stDerivative(
			const glm::vec3 points[],
			const size_t    count,
			const float     t
		);
	private:
		template <typename T>
		T cyclic_clamp(const T value, const T minBound, const T maxBound) {

			T amp = maxBound - minBound;
			T val = fmod(value - minBound, amp);

			if (val < T(0))
				val += amp;

			return val + minBound;
		}
		glm::vec3 evaluateCurveSegment(
			const glm::vec3& P0,
			const glm::vec3& P1,
			const glm::vec3& P2,
			const glm::vec3& P3,
			const float t
		);
		glm::vec3 evaluateCurveSegment_1stDerivative(
			const glm::vec3& P0,
			const glm::vec3& P1,
			const glm::vec3& P2,
			const glm::vec3& P3,
			const float t
		);
	}move;

	CosmoSuit(Model * model) {
		this->model = model;
		this->geometry.position = glm::vec3(1.0f, 1.0f, 0.0f);
		this->geometry.direction = glm::vec3(1.0f, 0.0f, 0.0f);
		this->geometry.color = glm::vec3(0.2f, 0.2f, 0.0f);
		initShaderProgram();
	}

	CosmoSuit(Model * model, glm::vec3 initPos, glm::vec3 initDir, glm::vec3 initColor = glm::vec3(0.0f, 0.0f, 0.0f)) {
		this->model = model;
		this->geometry.position = initPos;
		this->geometry.direction = initDir;
		this->geometry.color = initColor;
		initShaderProgram();
	}
	~CosmoSuit(
		//delete[] geometry;
	);
	/**
		Draws current model
		@param program - currently used program
		@param relector - reference to main Reflector
		@param pointLights - list of used point lights
		@param dirLights - list of used directional lights
		@param alarms - list of used alarms
		@param playAlarms - defines whether to render light from alarms or not
	  */
	void Draw(Program * program, Camera * camera, Reflector * reflector, GameObjectsList pointlights, GameObjectsList dirLights, GameObjectsList alarms, bool playAlarms);
private:
	bool initShaderProgram();
};

