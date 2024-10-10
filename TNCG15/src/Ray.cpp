#include "Ray.h"
#include "Polygon.h"

glm::vec3 Ray::getColor(glm::vec3 ray_col) {
	return hit_surface->color * ray_col;
}