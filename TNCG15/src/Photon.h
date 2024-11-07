#pragma once

#include <glm/glm.hpp>
#include "Ray.h"

class Photon
{
public:
	glm::dvec3 position;
	glm::dvec3 direction;
	glm::dvec3 flux;
	int photonID;

	Photon(glm::dvec3 pos, glm::dvec3 dir, glm::dvec3 flu, const int id) {
		position = pos;
		direction = -dir;
		flux = flu;
		photonID = id; //0=global, 1=shadow, 2=caustic (balding man)
	}

private:

};

