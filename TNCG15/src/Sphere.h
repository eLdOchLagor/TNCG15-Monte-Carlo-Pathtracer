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

		if (arg > 0.0)
		{
			double t1 = (-c2 + sqrt(arg)) / (2.0 * c1);
			double t2 = (-c2 - sqrt(arg)) / (2.0 * c1);
			double t = t1 < t2 ? t1 : t2;
			//double t = (t1 >= 0) && t1 < t2 ? t1 : (t2 >= 0) ? t2 : -1.0;
			//if (t < 0) return nullptr;
			
			normal = glm::normalize(r.start_point + r.direction * t - center);

			//r.radiance = color;
			return t;
		}
		/*else if (arg == 0) {
			double t1 = -c2 / (2.0 * c1);
			double t = t1 > 0.0 ? t1 : -1.0;
			if (t < 0) return nullptr;
			r.end_point = r.start_point + r.direction * t;
			normal = glm::normalize(r.end_point - center);

			r.radiance = color;
			return this;
		}*/
		
		return -1.0;
	}

	glm::dvec3 center;
	double radius;

private:

};

