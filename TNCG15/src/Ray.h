#pragma once
#include <glm/glm.hpp>

class Polygon;

	class Ray {
	public:
		
		Ray(const glm::vec3& start, glm::vec3 dir, glm::vec3 ray_col, Polygon* hit = nullptr, Ray* prev_r = nullptr, Ray* next_r = nullptr) : start_point{ start }, direction{ dir }, previous_ray{prev_r}, next_ray{next_r}, hit_surface{hit} {
			radiance = ray_col;
			end_point = glm::vec3(0, 0, 0);
		}

		glm::vec3 getColor(glm::vec3 ray_col);

		Ray* createNextRay() {};

		glm::vec3 start_point;
		glm::vec3 end_point;
		glm::vec3 direction;
		glm::vec3 radiance;
		Polygon* hit_surface;
		Ray* previous_ray;
		Ray* next_ray;
	private:

	};
