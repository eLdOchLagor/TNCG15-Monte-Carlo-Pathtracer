#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <span>
#include <math.h>
#include "Polygon.h"

namespace Rectangle {
	class Rectangle : public Polygon::Polygon{
	public:

		Rectangle(const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3, const glm::vec3& point4,const glm::vec3& col)   {
			Polygon::verticies.push_back(point1);
			Polygon::verticies.push_back(point2);
			Polygon::verticies.push_back(point3);
			Polygon::verticies.push_back(point4);
			color = col;
		}

	private:
		
	};

}

