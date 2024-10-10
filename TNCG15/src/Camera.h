#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Ray.h"


	class Camera
	{
	public:
		glm::vec3 position;  // Camera position
		glm::vec3 forward;   // Forward (look-at) vector
		glm::vec3 trueUp;        // Up vector
		glm::vec3 right;     // Right vector (perpendicular to forward and up)

		float fov;         // Field of view in radians
		float aspectRatio; // Aspect ratio of the image (width / height)
		float imagePlaneWidth, imagePlaneHeight; 

		Camera(glm::vec3 pos, glm::vec3 fwd, glm::vec3 up, float fov, float aspect) : position(pos), forward(fwd), fov(fov), aspectRatio(aspect) {

			right = glm::normalize(glm::cross(up, forward));
			trueUp = glm::cross(forward, right);
			imagePlaneHeight = 2.0f * tan(glm::radians(fov) / 2.0f);
			imagePlaneWidth = imagePlaneHeight * aspectRatio;

		}

		Ray shootStartRay(glm::vec3 eye, float u, float v) {
			//std::cout << right.y;
			glm::vec3 direction = ((forward + position) + u * imagePlaneWidth * right + v * imagePlaneHeight * trueUp) - eye;
			Ray firstRay = Ray(eye, direction, glm::vec3(1.0, 1.0, 1.0));

			return firstRay;
		}




	private:
		

	};



