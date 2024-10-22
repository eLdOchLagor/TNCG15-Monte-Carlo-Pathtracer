#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Ray.h"



	class Polygon {
	public:
		
		std::vector<glm::dvec3> verticies;
		glm::dvec3 color;
		glm::dvec3 normal;
		bool mirror;
		bool isBoundry;

		virtual Polygon* surfaceIntersectionTest(Ray& r) { return nullptr; };
		
	protected:
		
		


	private: 
		
	};


