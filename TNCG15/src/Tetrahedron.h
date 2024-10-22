#pragma once
#include "Polygon.h"
#include "Triangle.h"

class Tetrahedron : public Polygon {
public:
	Tetrahedron(const glm::dvec3& point1, const glm::dvec3& point2, const glm::dvec3& point3, const glm::dvec3& point4, const glm::dvec3& col, const bool mir = false, bool isB = false) {
		verticies.push_back(point1);
		verticies.push_back(point2);
		verticies.push_back(point3);
		verticies.push_back(point4);
		color = col;
		mirror = mir;
		isBoundry = isB;
		constructTriangles();
	}

	void constructTriangles() {
		Triangle tri1(verticies[0], verticies[1], verticies[2], color, false, true);
		Triangle tri2(verticies[0], verticies[3], verticies[1], color, false, true);
		Triangle tri3(verticies[0], verticies[2], verticies[3], color, false, true);
		Triangle tri4(verticies[2], verticies[1], verticies[3], color, false, true);
		triangles.push_back(tri1);
		triangles.push_back(tri2);
		triangles.push_back(tri3);
		triangles.push_back(tri4);
	}

	Polygon* surfaceIntersectionTest(Ray& r) override {
		for (Triangle temp : triangles) {
			Polygon* intersectedSurface = temp.surfaceIntersectionTest(r);

			
			if (intersectedSurface != nullptr) {
				std::cout << intersectedSurface->color.b << "\n";
				return intersectedSurface;
			}
		}

		return nullptr;
	}

private:
	std::vector<Triangle> triangles;
};

