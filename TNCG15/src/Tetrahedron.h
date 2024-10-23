#pragma once
#include "Polygon.h"
#include "Triangle.h"

class Tetrahedron : public Polygon {
public:
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

	void constructTriangles() {
		Triangle tri1(verticies[0], verticies[1], verticies[2], color, 2, 0.0, true);
		Triangle tri2(verticies[0], verticies[3], verticies[1], color, 2, 0.0, true);
		Triangle tri3(verticies[0], verticies[2], verticies[3], color, 2, 0.0, true);
		Triangle tri4(verticies[2], verticies[1], verticies[3], color, 2, 0.0, true);
		triangles.push_back(tri1);
		triangles.push_back(tri2);
		triangles.push_back(tri3);
		triangles.push_back(tri4);
	}

/*	std::pair<Polygon*, double> surfaceIntersectionTest(Ray& r) override {
		for (Triangle temp : triangles) {
			std::pair<Polygon*, double> intersectedSurface = temp.surfaceIntersectionTest(r);

			
			if (intersectedSurface.first != nullptr) {
				//std::cout << intersectedSurface->color.b << "\n";
				return intersectedSurface;
			}
		}

		return std::pair(nullptr, std::numeric_limits<double>::max());
	}

	*/

private:
	std::vector<Triangle> triangles;
};

