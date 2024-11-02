#pragma once
#include "Polygon.h"
#include "Triangle.h"

class Tetrahedron : public Polygon {
public:
	std::vector<Triangle*> triangles;

	Tetrahedron(const glm::dvec3& point1, const glm::dvec3& point2, const glm::dvec3& point3, const glm::dvec3& point4, const glm::dvec3& col, const int& surfID, const double reflec = 0.0, bool isB = false) {
		verticies.push_back(point1);
		verticies.push_back(point2);
		verticies.push_back(point3);
		verticies.push_back(point4);
		color = col;
		surfaceID = surfID;
		reflectance = reflec;
		isBoundry = isB;
		constructTriangles();

	}

	Tetrahedron* clone() const {
		return new Tetrahedron(*this);
	}

	Tetrahedron(Polygon* tetra) {
		/*verticies.push_back(tetra->verticies[0]);
		verticies.push_back(tetra->verticies[1]);
		verticies.push_back(tetra->verticies[2]);
		verticies.push_back(tetra->verticies[3]);
		color = tetra->color;
		surfaceID = surfID;
		reflectance = reflec;
		isBoundry = isB;
		constructTriangles();*/
		
	}

	
	void constructTriangles() {
		Triangle* tri1 = new Triangle(verticies[0], verticies[1], verticies[2], color, surfaceID, reflectance, isBoundry);
		Triangle* tri2 = new Triangle(verticies[0], verticies[3], verticies[1], color, surfaceID, reflectance, isBoundry);
		Triangle* tri3 = new Triangle(verticies[0], verticies[2], verticies[3], color, surfaceID, reflectance, isBoundry);
		Triangle* tri4 = new Triangle(verticies[2], verticies[1], verticies[3], color, surfaceID, reflectance, isBoundry);
		triangles.push_back(tri1);
		triangles.push_back(tri2);
		triangles.push_back(tri3);
		triangles.push_back(tri4);
	}

	double surfaceIntersectionTest(Ray& r) override {
		std::vector<double> intersectedSurface;
		for (Triangle* temp : triangles) {
			intersectedSurface.push_back(temp->surfaceIntersectionTest(r));
		}

		double closestT = -1;
		for (double t : intersectedSurface) {
			if ((closestT < 0.0 || t < closestT) && t>1e-6) {
				closestT = t;
			}
		}

		return closestT;
	}

	
	
private:
	
};

