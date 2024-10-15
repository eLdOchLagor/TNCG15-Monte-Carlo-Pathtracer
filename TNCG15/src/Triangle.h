#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <span>
#include <math.h>
#include "Polygon.h"



	class Triangle : public Polygon{
	public:

		Triangle(const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3, const glm::vec3& col, const bool mir = false) {
			verticies.push_back(point1);
			verticies.push_back(point2);
			verticies.push_back(point3);
			color = col;
			normal = glm::normalize(glm::cross(point2-point1, point3 - point1));
			mirror = mir;
		}

		Polygon* surfaceIntersectionTest(Ray& r) override {

			glm::vec3 d = glm::normalize(r.direction);
			glm::vec3 s = r.start_point;
			
			// If negative, then the surface is visible for the ray
			if (glm::dot(d, normal) < 0.0)
			{

				glm::vec3 c1 = verticies[1] - verticies[0];
				glm::vec3 c2 = verticies[2] - verticies[0];

				glm::vec3 P = glm::cross(d, c2);
				float det = glm::dot(c1, P);

				glm::vec3 T = s - verticies[0];
				float u = glm::dot(T, P) / det;

				glm::vec3 Q = glm::cross(T, c1);
				float v = glm::dot(d, Q) / det;

				float t = glm::dot(c2, Q) / det;

				glm::vec3 intersectionPoint = s + t * d;
				// If intersectionPoint is a valid point on the surface
				if (0.0 <= u && 0.0 <= v && u+v <=1.0 && t >= 0)
				{
					//r.hit_surface = this;
					r.end_point = intersectionPoint;
					r.radiance = color;
					return this;
				}

			}
			return nullptr;
		}
		
	private:

	};

	




