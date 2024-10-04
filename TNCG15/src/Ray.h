#include <glm/glm.hpp>


namespace Ray {	
	class Ray {
	public:
		
		Ray(const glm::vec3& start, const glm::vec3& end, Ray* next = nullptr) : start_point{ start }, end_point{ end }, next_ray{ next }{}


	private:
		
		glm::vec3 start_point;
		glm::vec3 end_point;
		Ray* next_ray;

	};
}