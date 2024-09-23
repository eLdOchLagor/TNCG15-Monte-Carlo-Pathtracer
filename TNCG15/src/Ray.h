#include <glm/glm.hpp>


namespace Ray {	
	class Ray {
	public:
		
		Ray(const glm::vec3& start,const glm::vec3& end) : start_point{ start }, end_point{ end } {}


	private:
		
		glm::vec3 start_point;
		glm::vec3 end_point;

	};
}