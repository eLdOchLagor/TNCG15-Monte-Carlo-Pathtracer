#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <random>
#include <span>
#include <math.h>
#include "Polygon.h"
#include "Triangle.h"


	class Rectangle : public Polygon{
	public:
		std::vector<Triangle*> triangles;
		Rectangle(const glm::dvec3& point1, const glm::dvec3& point2, const glm::dvec3& point3, const glm::dvec3& point4,const glm::dvec3& col,const int& surfID, const double reflec, bool isB = false)   {
			verticies.push_back(point1);
			verticies.push_back(point2);
			verticies.push_back(point3);
			verticies.push_back(point4);
			color = col;
			normal = glm::normalize(glm::cross(point2 - point1, point4 - point1));
			surfaceID = surfID;
			reflectance = reflec;
			isBoundry = isB;
			constructTriangles();
		}

		void constructTriangles() {
			Triangle* tri1 = new Triangle(verticies[0], verticies[1], verticies[2], color, surfaceID, reflectance, isBoundry);
			Triangle* tri2 = new Triangle(verticies[0], verticies[2], verticies[3], color, surfaceID, reflectance, isBoundry);
			
			triangles.push_back(tri1);
			triangles.push_back(tri2);

		}

		double surfaceIntersectionTest(Ray& r, const bool shadowPhoton = false) override {

			std::vector<double> intersectedSurface;
			for (Triangle* temp : triangles) {
				intersectedSurface.push_back(temp->surfaceIntersectionTest(r,shadowPhoton));
			}

			double closestT = -1;
			for (double t : intersectedSurface) {
				if ((closestT < 0.0 || t < closestT) && t > 1e-6) {
					closestT = t;
				}
			}

			return closestT;
		}

	private:
		
	};



