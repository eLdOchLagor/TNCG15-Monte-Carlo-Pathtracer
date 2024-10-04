#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <span>
#include <math.h>
#include "Polygon.h"


namespace Triangle {
	
	class Triangle : public Polygon::Polygon{
	public:

		Triangle(const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3, const glm::vec3& col) {
			Polygon::verticies.push_back(point1);
			Polygon::verticies.push_back(point2);
			Polygon::verticies.push_back(point3);
			color = col;
		}

		
	private:

	};

	


}

