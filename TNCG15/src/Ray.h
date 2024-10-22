#pragma once
#include <glm/glm.hpp>

class Polygon;

	class Ray {
	public:
		
		Ray(const glm::dvec3& start, glm::dvec3 dir, glm::dvec3 ray_col, Polygon* hit = nullptr, Ray* prev_r = nullptr, Ray* next_r = nullptr) : start_point{ start }, direction{ glm::normalize(dir) }, previous_ray{prev_r}, next_ray{next_r}, hit_surface{hit} {
			radiance = ray_col;
			end_point = glm::dvec3(0, 0, 0);
			
		}

		glm::dvec3 getColor(glm::dvec3 ray_col);

		Ray* createNextRay() {};

		glm::dvec3 start_point;
		glm::dvec3 end_point;
		glm::dvec3 direction;
		glm::dvec3 radiance;		
		Polygon* hit_surface;
		Ray* previous_ray;
		Ray* next_ray;

	private:

	};
