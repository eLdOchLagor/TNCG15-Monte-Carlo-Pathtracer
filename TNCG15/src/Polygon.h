#pragma once
#include <glm/glm.hpp>
#include <vector>


	class Polygon {
	public:
		
		std::vector<glm::vec3> verticies;
		glm::vec3 color;
		glm::vec3 normal;

		

		const std::vector<glm::vec3>& get_verticies() {
			return verticies;
		}
		
	protected:
		
		


	private: 
		
	};


