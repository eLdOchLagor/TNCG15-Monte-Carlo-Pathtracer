#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <span>
#include <math.h>
#include "Polygon.h"



	class Triangle : public Polygon{
	public:

		Triangle(const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3, const glm::vec3& col) {
			verticies.push_back(point1);
			verticies.push_back(point2);
			verticies.push_back(point3);
			color = col;
			normal = glm::cross(point3 - point1, point2 - point1);
		}

		
	private:

	};

	




