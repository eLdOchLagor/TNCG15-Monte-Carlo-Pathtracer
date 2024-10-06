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
			normal = glm::cross(point3 - point1, point2 - point1);
		}

		Polygon* surfaceIntersectionTest(glm::vec3 start_point, glm::vec3 dir) {

		}

	private:
		
	};



