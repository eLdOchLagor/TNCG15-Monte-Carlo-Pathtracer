#include <glm/glm.hpp>


namespace Polygon {
	
	class Polygon {
	public:

		const std::vector<glm::vec3>& get_verticies() {
			return verticies;
		}

	protected:
		std::vector<glm::vec3> verticies;
	private: 
		
	};


}