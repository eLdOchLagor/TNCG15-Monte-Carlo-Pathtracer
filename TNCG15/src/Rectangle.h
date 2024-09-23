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

		Rectangle(const std::vector<glm::vec3>& all_points) {Polygon::verticies = all_points; }

	private:
		
	};

}

