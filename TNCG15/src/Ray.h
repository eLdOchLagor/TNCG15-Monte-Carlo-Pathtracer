#pragma once
#include <glm/glm.hpp>
#include "Polygon.h"



	class Ray {
	public:
		
		Ray(const glm::vec3& start, const glm::vec3& end, Polygon* hit, glm::vec3 ray_col, Ray* prev_r = nullptr, Ray* next_r = nullptr) : start_point{ start }, end_point{ end }, previous_ray{prev_r}, next_ray{next_r}, hit_surface{hit} {
			direction = calculateDirection(start, end);
			radiance = getColor(ray_col);
		}

		glm::vec3 calculateDirection(const glm::vec3& start, const glm::vec3& end) { return glm::normalize(end - start); }

		glm::vec3 getColor(glm::vec3 ray_col) {
			return hit_surface->color * ray_col; 
		}

	private:
		
		glm::vec3 start_point;
		glm::vec3 end_point;
		glm::vec3 direction;
		glm::vec3 radiance;
		Polygon* hit_surface;
		Ray* previous_ray;
		Ray* next_ray;

	};
