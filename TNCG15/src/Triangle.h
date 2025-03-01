#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <span>
#include <math.h>
#include "Polygon.h"



class Triangle : public Polygon {
public:

	Triangle(const glm::dvec3& point1, const glm::dvec3& point2, const glm::dvec3& point3, const glm::dvec3& col,const int& surfID, const double reflec, bool isB = false) {
		verticies.push_back(point1);
		verticies.push_back(point2);
		verticies.push_back(point3);
		color = col;
		normal = glm::normalize(glm::cross(point2 - point1, point3 - point1));
		surfaceID = surfID;
		reflectance = reflec;
		isBoundry = isB;
	}

	double surfaceIntersectionTest(Ray& r,const bool shadowPhoton = false) override {

		glm::dvec3 d = glm::normalize(r.direction);
		glm::dvec3 s = r.start_point;

		// If negative, then the surface is visible for the ray
		if (glm::dot(d, normal) < 0.0 || shadowPhoton)
		{

			glm::dvec3 c1 = verticies[1] - verticies[0];
			glm::dvec3 c2 = verticies[2] - verticies[0];

			glm::dvec3 P = glm::cross(d, c2);
			double det = glm::dot(c1, P);
			//if (det < 1e-8 && det > -1e-8) {
			//	return -1.0;
			//}
			glm::dvec3 T = s - verticies[0];
			double u = glm::dot(T, P) / det;
			if (u < 0 || u > 1) {
				return -1.0;
			}

			glm::dvec3 Q = glm::cross(T, c1);
			double v = glm::dot(d, Q) / det;
			if (v < 0 || v > 1-u) {
				return -1.0;
			}
			double t = glm::dot(c2, Q) / det;
			if (t < 0) {
				return -1.0;
			}
			return t;
			
			// If intersectionPoint is a valid point on the surface
			//if (u + v <= 1.0)
			//{
				//glm::dvec3 intersectionPoint = s + t * d;
				//r.hit_surface = this;
				//r.end_point = intersectionPoint;
				//r.radiance = color;
			
			//}

		}
		return -1.0;
	}

private:

};






