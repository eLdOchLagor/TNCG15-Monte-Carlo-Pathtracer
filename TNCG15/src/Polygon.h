#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Ray.h"
#include <utility>



	class Polygon {
	public:
		virtual Polygon* clone() const = 0;

		std::vector<glm::dvec3> verticies;
		glm::dvec3 color;
		glm::dvec3 normal;
		int surfaceID; // 0 = lights, 1 = mirror, 2 = diffuse, 3 = transparent
		double reflectance;
		bool isBoundry;

		virtual double surfaceIntersectionTest(Ray& r) = 0;
		
	protected:
		
		


	private: 
		
	};


