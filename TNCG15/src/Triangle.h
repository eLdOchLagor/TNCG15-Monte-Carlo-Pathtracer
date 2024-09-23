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


		Triangle(std::vector<double> point);

		std::vector<double> get_Points() {
			return points;
		}
	private:
		std::vector<double> points;
	};

	


}

