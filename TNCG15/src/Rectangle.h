#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <span>
#include <math.h>
#include "Polygon.h"


	class Rectangle : public Polygon{
	public:

		Rectangle(const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3, const glm::vec3& point4,const glm::vec3& col)   {
			verticies.push_back(point1);
			verticies.push_back(point2);
			verticies.push_back(point3);
			verticies.push_back(point4);
			color = col;
			normal = glm::cross(point2 - point1, point4 - point1);
		}

		void surfaceIntersectionTest(Ray& r) override {

			glm::vec3 d = glm::normalize(r.direction);
			glm::vec3 s = r.start_point;
			
			// If negative, then the surface is visible for the ray
			if (glm::dot(d, normal) < 0.0)
			{
				float t = glm::dot(verticies[0] - s, normal / (glm::dot(d, normal)));
				glm::vec3 intersectionPoint = s + t * d;
				
				glm::vec3 c1 = verticies[1] - verticies[0];
				glm::vec3 c2 = verticies[3] - verticies[0];

				float a = glm::dot(intersectionPoint - verticies[0], c1) / glm::dot(c1, c1);
				float b = glm::dot(intersectionPoint - verticies[0], c2) / glm::dot(c2, c2);

				// If intersectionPoint is a valid point on the surface
				if (0.0 <= a && a <= 1.0 && 0.0 <= b && b <= 1.0)
				{
					//std::cout << a << " " << b << "\n";
					//std::cout << "Hit a surface";
					r.hit_surface = this;
					r.end_point = intersectionPoint;
					r.radiance = color;
					//std::cout << color.g;
					//std::cout << a << " " << b << "\n";
				}
				
			}
		}

	private:
		
	};



