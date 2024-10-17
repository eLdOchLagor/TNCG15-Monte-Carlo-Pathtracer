#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Ray.h"
#include <fstream>


	class Camera
	{
	public:
		glm::vec3 position;  // Camera position
		glm::vec3 forward;   // Forward (look-at) vector
		glm::vec3 trueUp;        // Up vector
		glm::vec3 right;     // Right vector (perpendicular to forward and up)

		float fov;         // Field of view in radians
		float aspectRatio; // Aspect ratio of the image (width / height)
		float imagePlaneWidth, imagePlaneHeight; 

		std::vector<Polygon*> objects;

		Camera(glm::vec3 pos, glm::vec3 fwd, glm::vec3 up, float fov, float aspect, std::vector<Polygon*> obj) : position(pos), forward(fwd), fov(fov), aspectRatio(aspect), objects(obj) {

			right = glm::normalize(glm::cross(forward, up));
			trueUp = glm::cross(right, forward);
			imagePlaneHeight = 2.0f * tan(glm::radians(fov) / 2.0f);
			imagePlaneWidth = imagePlaneHeight * aspectRatio;
			

		}

		Ray shootStartRay(glm::vec3 eye, float u, float v) {
			//std::cout << right.y;
			glm::vec3 direction = ((forward + position) + u * imagePlaneWidth * right + v * imagePlaneHeight * trueUp) - eye;
			Ray firstRay = Ray(eye, direction, glm::vec3(1.0, 1.0, 1.0));

			return firstRay;
		}

		void shootNextRay(Ray& firstRay) {
			size_t i = 0;
			Ray* previousRay = &firstRay;
			Ray* tempRay = previousRay;
			while (i < 5) {
				for (Polygon* temp : objects)
				{

					Polygon* surface = temp->surfaceIntersectionTest(*previousRay);
					if (surface != nullptr) {
						previousRay->hit_surface = surface;
						break;
					}

				}

				if ((*previousRay).hit_surface->mirror == 1) {
					//std::cout << "prickade en mirror";
					glm::vec3 d_o = glm::normalize(previousRay->direction) - 2 * glm::dot(glm::normalize(previousRay->direction), previousRay->hit_surface->normal) * previousRay->hit_surface->normal;
					glm::vec3 startPoint = previousRay->end_point;
					glm::vec3 importance = previousRay->radiance;
					Ray newRay{ startPoint, d_o, importance };
					previousRay->next_ray = &newRay;
					//tempRay = previousRay->next_ray;
					newRay.previous_ray = previousRay;
					previousRay = previousRay->next_ray;
					//shootNextRay(newRay);
					//prevRay.radiance = newRay.radiance;
					i++;
				}
				else if ((*previousRay).hit_surface->mirror == 0) {

					(*previousRay).radiance = (*previousRay).hit_surface->color;

					i = 0;
					break;
				}
				else {
					
					i = 0;
					break;
				}
			}
			
			
			
		}

		glm::vec3 calcColor(Ray& firstRay) {
			Ray* currentRay = &firstRay;
			glm::vec3 color = currentRay->radiance;
			while (currentRay != nullptr) {
				if (currentRay->hit_surface->mirror == 1) {
					currentRay = currentRay->next_ray;
				}
				else {
					color = currentRay->radiance;
					currentRay = currentRay->next_ray;
				}
			}
			return color;
		}

		void render(const std::vector<Polygon*>& objects, int width, int height) {
			std::vector<std::vector<glm::vec3>> frameBuffer;

			// Create and open a text file
			std::ofstream OutputFile("render.ppm");

			// Setup PPM file settings
			OutputFile << "P3\n# This is a render!\n" << width << " " << height << "\n255\n";

			//Create image-matrix from raytrace
			for (size_t z = 0; z < height; z++) {
				std::clog << "\rScanlines remaining: " << (height - z) << ' ' << std::flush;
				std::vector<glm::vec3> row;
				for (size_t y = 0; y < width; y++) {

					float u = (2.0f * (y + 0.5f) / width) - 1.0f;
					float v = 1.0f - (2.0f * (z + 0.5f) / height);

					Ray firstRay = shootStartRay(glm::vec3(-1.0, 0.0, 0.0), u, v);
					shootNextRay(firstRay);
					glm::vec3 pixelColor = calcColor(firstRay);
					
					
					
					

					//std::cout << firstRay.radiance.x << " " << firstRay.radiance.y << " " << firstRay.radiance.z << "\n";

					row.push_back(pixelColor); //TODO: replace with generateRay
					//row.push_back((y % 2 == 0 ? glm::vec3(0, 0, 0) : glm::vec3(255, 255, 255))); //TODO: replace with generateRay
				}
				frameBuffer.push_back(row);
			}

			// Find largest double in all pixels to normalize with
			double largest = std::numeric_limits<double>::lowest();

			for (const auto& row : frameBuffer) {
				for (const auto& vec : row) {
					largest = std::max(largest, static_cast<double>(vec.x));
					largest = std::max(largest, static_cast<double>(vec.y));
					largest = std::max(largest, static_cast<double>(vec.z));
				}
			}

			//Write imageFile from frameBuffer
			for (size_t y = 0; y < height; y++) {

				for (size_t x = 0; x < width; x++) 
				{
					OutputFile << (int)((frameBuffer[y][x][0] / largest) * 255) << " ";
					OutputFile << (int)((frameBuffer[y][x][1] / largest) * 255) << " ";
					OutputFile << (int)((frameBuffer[y][x][2] / largest) * 255) << " ";
				}
				OutputFile << "\n";
			}
			// Close the file
			OutputFile.close();
		}




	private:
		

	};



