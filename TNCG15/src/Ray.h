#pragma once
#include <glm/glm.hpp>
#include <iostream>

class Polygon;

	class Ray {
	public:
		
		Ray(const glm::dvec3& start, glm::dvec3 dir, glm::dvec3 ray_col, Polygon* hit = nullptr, Ray* prev_r = nullptr, Ray* next_r = nullptr) : start_point{ start }, direction{ glm::normalize(dir) }, previous_ray{prev_r}, next_ray{next_r}, hit_surface{hit} {
			radiance = ray_col;
			end_point = glm::dvec3(0, 0, 0);
			
		}
		~Ray() {
			hit_surface = nullptr;
			previous_ray = nullptr;
			next_ray = nullptr;
			
		}

		void dealocateRay(Ray& ray){
			Ray* currentptr = &ray;
			
			Ray* nextptr = nullptr;

			while (currentptr != nullptr) {
				nextptr = currentptr->next_ray;
				
				delete currentptr;
				
				currentptr = nextptr;
			}
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
		int depth = 0;

	private:

	};
