#include "Ray.h"
#include "Polygon.h"

glm::dvec3 Ray::getColor(glm::dvec3 ray_col) {
	return hit_surface->color * ray_col;
}