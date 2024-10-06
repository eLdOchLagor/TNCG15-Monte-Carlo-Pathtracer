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
		}

	private:
		
	};



