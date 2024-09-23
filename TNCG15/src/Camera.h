#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace Camera {
	class Camera
	{
	public:
		Camera(const std::vector<glm::vec3>& corners, const glm::vec3& eye_pos, const int& resx, const int& resy) : corner_points{corners},
			eye_position{ eye_pos }, resolution_x{ resx }, resolution_y{resy} {
			
		}



	private:
		std::vector<glm::vec3> corner_points;
		glm::vec3 eye_position;
		int resolution_x;
		int resolution_y;
		std::vector<std::vector<glm::vec4>> pixel_values;

	};
}


