#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <random>
#include <span>
#include <math.h>
#include "Polygon.h"


	class Rectangle : public Polygon{
	public:

		Rectangle(const glm::dvec3& point1, const glm::dvec3& point2, const glm::dvec3& point3, const glm::dvec3& point4,const glm::dvec3& col,const int& surfID, const double reflec, bool isB = false)   {
			verticies.push_back(point1);
			verticies.push_back(point2);
			verticies.push_back(point3);
			verticies.push_back(point4);
			color = col;
			normal = glm::normalize(glm::cross(point2 - point1, point4 - point1));
			surfaceID = surfID;
			reflectance = reflec;
			isBoundry = isB;
		}

		std::pair<Polygon*, double> surfaceIntersectionTest(Ray& r) override {

			glm::dvec3 d = glm::normalize(r.direction);
			glm::dvec3 s = r.start_point;
			
			// If negative, then the surface is visible for the ray
			if (glm::dot(d, normal) < 0.0)
			{
				double t = glm::dot(verticies[0] - s, normal / (glm::dot(d, normal)));
				glm::dvec3 intersectionPoint = s + t * d;
				
				glm::dvec3 c1 = verticies[1] - verticies[0];
				glm::dvec3 c2 = verticies[3] - verticies[0];

				double a = glm::dot(intersectionPoint - verticies[0], c1) / glm::dot(c1, c1);
				double b = glm::dot(intersectionPoint - verticies[0], c2) / glm::dot(c2, c2);

				// If intersectionPoint is a valid point on the surface
				if (0.0 <= a && a <= 1.0 && 0.0 <= b && b <= 1.0)
				{
					//r.hit_surface = this;
					r.end_point = intersectionPoint;
					r.radiance = color;
					return std::pair(this,t);
				}
				
			}
			return std::pair(nullptr, std::numeric_limits<double>::max());
		}

	private:
		
	};



