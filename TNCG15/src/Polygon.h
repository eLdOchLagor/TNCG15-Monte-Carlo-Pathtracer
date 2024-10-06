#pragma once
#include <glm/glm.hpp>



	class Polygon {
	public:
		
		

		

		const std::vector<glm::vec3>& get_verticies() {
			return verticies;
		}

	protected:
		
		std::vector<glm::vec3> verticies;
		glm::vec3 color;

	private: 
		
	};


