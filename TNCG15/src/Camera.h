#pragma once

#include <vector>
#include <glm/glm.hpp>


	class Camera
	{
	public:
		glm::vec3 position;  // Camera position
		glm::vec3 forward;   // Forward (look-at) vector
		glm::vec3 up;        // Up vector
		glm::vec3 right;     // Right vector (perpendicular to forward and up)

		float fov;         // Field of view in radians
		float aspectRatio; // Aspect ratio of the image (width / height)
		float imagePlaneWidth, imagePlaneHeight; 

		Camera(glm::vec3 pos, glm::vec3 fwd, glm::vec3 up, float fov, float aspect) : position(pos), forward(fwd), up(up), fov(fov), aspectRatio(aspect) {

			right = glm::normalize(glm::cross(forward, up));
			
			imagePlaneHeight = 2.0f * tan(fov / 2.0f);
			imagePlaneWidth = imagePlaneHeight * aspectRatio;

		}




	private:
		

	};



