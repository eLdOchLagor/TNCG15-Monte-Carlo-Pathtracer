#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Ray.h"



	class Polygon {
	public:
		
		std::vector<glm::vec3> verticies;
		glm::vec3 color;
		glm::vec3 normal;
		bool mirror;

		virtual Polygon* surfaceIntersectionTest(Ray& r) { return nullptr; };
		
	protected:
		
		


	private: 
		
	};


