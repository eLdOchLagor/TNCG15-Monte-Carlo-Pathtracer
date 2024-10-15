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

		Camera(glm::vec3 pos, glm::vec3 fwd, glm::vec3 up, float fov, float aspect) : position(pos), forward(fwd), fov(fov), aspectRatio(aspect) {

			right = glm::normalize(glm::cross(up, forward));
			trueUp = glm::cross(forward, right);
			imagePlaneHeight = 2.0f * tan(glm::radians(fov) / 2.0f);
			imagePlaneWidth = imagePlaneHeight * aspectRatio;

		}

		Ray shootStartRay(glm::vec3 eye, float u, float v) {
			//std::cout << right.y;
			glm::vec3 direction = ((forward + position) + u * imagePlaneWidth * right + v * imagePlaneHeight * trueUp) - eye;
			Ray firstRay = Ray(eye, direction, glm::vec3(1.0, 1.0, 1.0));

			return firstRay;
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

					for (Polygon* temp : objects)
					{
						temp->surfaceIntersectionTest(firstRay);
					}

					//std::cout << firstRay.radiance.x << " " << firstRay.radiance.y << " " << firstRay.radiance.z << "\n";

					row.push_back(firstRay.radiance); //TODO: replace with generateRay
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

				for (size_t x = 0; x < width; x++) {

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



