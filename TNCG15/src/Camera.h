#pragma once

#define _USE_MATH_DEFINES
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"
#include <cmath>
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
		int widthPixels, heightPixels;

		Rectangle* light = new Rectangle(glm::vec3(-2, -2, 5), glm::vec3(-2, 2, 5), glm::vec3(2, 2, 5), glm::vec3(2, -2, 5), glm::vec3(1.0, 1.0, 1.0));

		std::vector<Polygon*> objects;

		Camera(glm::vec3 pos, glm::vec3 fwd, glm::vec3 up, float fov, int width, int height, std::vector<Polygon*> obj) : position(pos), forward(fwd), fov(fov), widthPixels{width}, heightPixels{height}, objects(obj) {
			aspectRatio = (float)width / height;
			right = glm::normalize(glm::cross(forward, up));
			trueUp = glm::cross(right, forward);
			imagePlaneHeight = 2.0f * tan(glm::radians(fov) / 2.0f);
			imagePlaneWidth = imagePlaneHeight * aspectRatio;
		}

		Ray shootStartRay(glm::vec3 eye, float u, float v) {
			//std::cout << right.y;
			glm::vec3 direction = glm::normalize(forward + u * right + v * trueUp);
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

				if (previousRay->hit_surface->mirror == 1) {
					//std::cout << "prickade en mirror";
					glm::vec3 d_o = glm::normalize(previousRay->direction) - 2 * glm::dot(glm::normalize(previousRay->direction), previousRay->hit_surface->normal) * previousRay->hit_surface->normal;
					glm::vec3 startPoint = previousRay->end_point;
					glm::vec3 importance = previousRay->radiance;
					Ray* newRay = new Ray(startPoint, d_o, importance);
					previousRay->next_ray = newRay;
					//tempRay = previousRay->next_ray;
					newRay->previous_ray = previousRay;
					previousRay = previousRay->next_ray;
					//shootNextRay(newRay);
					//prevRay.radiance = newRay.radiance;

					i++;
				}
				else if (previousRay->hit_surface->mirror == 0) {

					glm::dvec3 e1 = light->verticies[1] - light->verticies[0];
					glm::dvec3 e2 = light->verticies[3] - light->verticies[0];

					glm::dvec3 radiance(0.0, 0.0, 0.0);
					int N = 15;
					glm::dvec3 surfaceColor = previousRay->hit_surface->color;

					for (size_t i = 0; i < N; i++)
					{
						double s = (double)rand() / RAND_MAX;
						double t = (double)rand() / RAND_MAX;

						glm::dvec3 y = glm::dvec3(light->verticies[0]) + s * e1 + t * e2;
						glm::dvec3 di = y - glm::dvec3(previousRay->end_point);
						
						double cosx = glm::dot(glm::dvec3(previousRay->hit_surface->normal), di / glm::length(di));
						double cosy = glm::dot(-glm::dvec3(light->normal), di / glm::length(di));

						double scalar_radiance = (cosx * cosy) / (glm::length(di) * glm::length(di));

						radiance += surfaceColor * scalar_radiance;
					}

					radiance *= 16.0 / (2*acos(0.0) * N); // 2*acos(0.0) = pi
					
					previousRay->radiance = radiance;

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

		void render() {
			std::vector<std::vector<glm::vec3>> frameBuffer;

			//Create image-matrix from raytrace
			for (size_t z = 0; z < heightPixels; z++) {
				std::clog << "\rScanlines remaining: " << (heightPixels - z) << ' ' << std::flush;
				std::vector<glm::vec3> row;
				for (size_t y = 0; y < widthPixels; y++) {

					float u = ((y + 0.5f) / widthPixels) * imagePlaneWidth - imagePlaneWidth / 2;
					float v = (1.0f - (z + 0.5f) / heightPixels) * imagePlaneHeight - imagePlaneHeight / 2;

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

			// Create a buffer to hold the final image data (unsigned char)
			std::vector<unsigned char> imageBuffer(widthPixels * heightPixels * 3);  // 3 bytes per pixel (RGB)

			//Write imageFile from frameBuffer
			for (size_t y = 0; y < heightPixels; y++) {

				for (size_t x = 0; x < widthPixels; x++) 
				{
					size_t index = (y * widthPixels + x) * 3;
					imageBuffer[index + 0] = static_cast<unsigned char>((frameBuffer[y][x][0] / largest) * 255);  // R
					imageBuffer[index + 1] = static_cast<unsigned char>((frameBuffer[y][x][1] / largest) * 255);  // G
					imageBuffer[index + 2] = static_cast<unsigned char>((frameBuffer[y][x][2] / largest) * 255);  // B
				}
			}
			
			// Write the buffer to a PNG file
			stbi_write_png("render.png", widthPixels, heightPixels, 3, imageBuffer.data(), widthPixels * 3);
		}

	private:
		

	};



