#pragma once
#include "Polygon.h"
#include <glm/glm.hpp>

class Sphere : public Polygon {
public:

	Sphere(glm::dvec3 cen, double r, glm::dvec3 col, const int& surfID, const double reflec, bool isB = false) : center{ cen }, radius{ r } {
		color = col;
		surfaceID = surfID;
		reflectance = reflec;
		isBoundry = isB;
	}

	double surfaceIntersectionTest(Ray& r) override {
		double c1 = glm::dot(r.direction, r.direction);
		double c2 = 2.0 * glm::dot(r.direction, r.start_point - center);
		double c3 = glm::dot(r.start_point - center, r.start_point - center) - radius * radius;

		double arg = c2 * c2 - 4.0 * c1 * c3;

		if (arg >= 0.0)
		{
			double t1 = (-c2 + sqrt(arg)) / (2.0 * c1);
			double t2 = (-c2 - sqrt(arg)) / (2.0 * c1);
			double t = t1 < t2 ? t1 : t2;
			
			glm::dvec3 end_point = r.start_point + r.direction * t;
			normal = glm::normalize(end_point - center);

			// Ensure normal is facing towards the ray, to work for refractions inside object
			if (glm::dot(r.direction, normal) > 0) {
				normal = -normal;  // Flip the normal
			}

			//r.radiance = color;

			return t;
		}
		
		return -1.0;
	}

	glm::dvec3 center;
	double radius;

private:

};

