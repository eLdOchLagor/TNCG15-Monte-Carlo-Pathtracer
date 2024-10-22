#pragma once

#define _USE_MATH_DEFINES
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"
#include <random>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Ray.h"
#include <fstream>


	class Camera
	{
	public:
		glm::dvec3 position;  // Camera position
		glm::dvec3 forward;   // Forward (look-at) vector
		glm::dvec3 trueUp;        // Up vector
		glm::dvec3 right;     // Right vector (perpendicular to forward and up)

		double fov;         // Field of view in radians
		double aspectRatio; // Aspect ratio of the image (width / height)
		double imagePlaneWidth, imagePlaneHeight;
		int widthPixels, heightPixels;
		int i = 0;
		Rectangle* light = new Rectangle(glm::dvec3(-2, -2, 5), glm::dvec3(-2, 2, 5), glm::dvec3(2, 2, 5), glm::dvec3(2, -2, 5), glm::dvec3(1.0, 1.0, 1.0), 0, 0);

		std::vector<Polygon*> objects;
		std::vector<Polygon*> sceneObjects;

		Camera(glm::dvec3 pos, glm::dvec3 fwd, glm::dvec3 up, double fov, int width, int height, std::vector<Polygon*> obj, std::vector<Polygon*> sceObj) : position(pos), forward(fwd), fov(fov), widthPixels{ width }, heightPixels{ height }, objects(obj), sceneObjects{sceObj} {
			aspectRatio = (double)width / height;
			right = glm::normalize(glm::cross(forward, up));
			trueUp = glm::cross(right, forward);
			imagePlaneHeight = 2.0f * tan(glm::radians(fov) / 2.0f);
			imagePlaneWidth = imagePlaneHeight * aspectRatio;
		}

		Ray shootStartRay(glm::dvec3 eye, double u, double v) {
			//std::cout << right.y;
			glm::dvec3 direction = glm::normalize(forward + u * right + v * trueUp);
			Ray firstRay = Ray(eye, direction, glm::dvec3(1.0, 1.0, 1.0));

			return firstRay;
		}

		void shootNextRay(Ray& firstRay) {
			//std::cout << i << "\n";
			Ray* previousRay = &firstRay;
			
				
				for (Polygon* temp : objects)
				{
					Polygon* surface = temp->surfaceIntersectionTest(*previousRay);
					if (surface != nullptr) {
						previousRay->hit_surface = surface;
						break;
					}
				}

				if (previousRay->hit_surface->surfaceID == 1) {
					//std::cout << "prickade en mirror";
					glm::dvec3 d_o = glm::normalize(previousRay->direction) - 2 * glm::dot(glm::normalize(previousRay->direction), previousRay->hit_surface->normal) * previousRay->hit_surface->normal;
					glm::dvec3 startPoint = previousRay->end_point;
					glm::dvec3 importance = previousRay->radiance;
					Ray* newRay = new Ray(startPoint, d_o, importance);
					previousRay->next_ray = newRay;
					//tempRay = previousRay->next_ray;
					newRay->previous_ray = previousRay;
					
					//shootNextRay(newRay);
					//prevRay.radiance = newRay.radiance;

					
				}
				else if (previousRay->hit_surface->surfaceID == 2) {
					
					std::random_device rd;   // Obtain a random seed
					std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
					std::uniform_real_distribution<> dis(0.0, 1.0);
					
					double Roh = 0.7;

					double randomValue = dis(gen);
					double randAzimuth = acos(sqrt(1 - randomValue));
					double randPhi = 2 * M_PI * randomValue;
					double rr = randPhi / Roh;
					if (rr <= 2 * M_PI) {

						double x = cos(randPhi) * sin(randAzimuth);
						double y = sin(randPhi) * sin(randAzimuth);
						double z = cos(randAzimuth);
						glm::dvec3 tangent, bitangent;
						glm::dvec3 normal = previousRay->hit_surface->normal;
						
						tangent =glm::normalize( - previousRay->direction + glm::dot(normal, previousRay->direction) * normal);
						
						
						bitangent = glm::normalize(glm::cross(normal, tangent));

						glm::dvec3 worldDir = glm::normalize(normal * z + tangent * x + bitangent * y);
	
						glm::dvec3 startPoint = previousRay->end_point;
						glm::dvec3 importance = previousRay->radiance;
						Ray* newRay = new Ray(startPoint, worldDir, importance);
						previousRay->next_ray = newRay;
						//tempRay = previousRay->next_ray;
						newRay->previous_ray = previousRay;
						i++;
						shootNextRay(*newRay);
					}
					//If we hit a diffuse surface and russian roulette determines that the ray is killed then thoust is the final if-statement determening 
					//the destiny of the radiance.
					else {
						Ray* rayPath = previousRay;
						rayPath->radiance = calculateDirectIllumination(rayPath);
						while (rayPath->previous_ray != nullptr) {
							if (rayPath->previous_ray->hit_surface->surfaceID == 1) {
								rayPath->previous_ray->radiance = rayPath->radiance;
								rayPath = rayPath->previous_ray;
							}
							else {
								rayPath->previous_ray->radiance = glm::dvec3(rayPath->previous_ray->hit_surface->color.r * rayPath->radiance.r,  
									rayPath->previous_ray->hit_surface->color.g * rayPath->radiance.g, 
									rayPath->previous_ray->hit_surface->color.b * rayPath->radiance.b) +
									calculateDirectIllumination(rayPath->previous_ray);
								rayPath = rayPath->previous_ray;
							}
						}
					}
					//previousRay->radiance = calculateDirectIllumination(previousRay);	
				}
				else {

					
					
				}
			
		}
		glm::dvec3 calculateDirectIllumination(Ray* ray) {
			glm::dvec3 e1 = light->verticies[1] - light->verticies[0];
			glm::dvec3 e2 = light->verticies[3] - light->verticies[0];

			glm::dvec3 radiance(0.0, 0.0, 0.0);
			int N = 15;
			glm::dvec3 surfaceColor = ray->hit_surface->color;

			for (size_t n = 0; n < N; n++)
			{
				double s = (double)rand() / RAND_MAX;
				double t = (double)rand() / RAND_MAX;

				glm::dvec3 y = glm::dvec3(light->verticies[0]) + s * e1 + t * e2;
				glm::dvec3 di = y - glm::dvec3(ray->end_point);

				double cosx = glm::dot(ray->hit_surface->normal, glm::normalize(di));
				double cosy = glm::dot(-light->normal, glm::normalize(di));

				// Make sure that surfaces facing away from the lightsource dont give negative values, these values give wrong result
				cosx = std::max(0.0, cosx);
				cosy = std::max(0.0, cosy);

				const double epsilon = 1e-4;
				if (!isInShadow(ray->end_point + epsilon * ray->hit_surface->normal, y, ray->hit_surface)) {
					double scalar_radiance = (cosx * cosy) / (glm::length(di) * glm::length(di));
					radiance += scalar_radiance;
				}

			}

			return radiance *= surfaceColor * 16.0 / (M_PI * N);
		}
		bool isInShadow(const glm::dvec3& point, const glm::dvec3& lightPos, const Polygon* originSurface) {
			Ray shadowRay(point, glm::normalize(lightPos - point), glm::dvec3(0,0,0));

			for (Polygon* obj : sceneObjects) {
				if (obj == originSurface)
				{
					continue;
				}

				if (obj->surfaceIntersectionTest(shadowRay)) {
					return true;  // Something is blocking the light
				}
			}
			return false;  // Light is visible
		}

		glm::dvec3 calcColor(Ray& firstRay) {
			Ray* currentRay = &firstRay;
			glm::dvec3 color = currentRay->radiance;
			while (currentRay != nullptr) {
				if (currentRay->hit_surface->surfaceID == 0) {
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
			std::vector<std::vector<glm::dvec3>> frameBuffer;

			//Create image-matrix from raytrace
			int samples = 2000;
			for (size_t z = 0; z < heightPixels; z++) {
				std::clog << "\rScanlines remaining: " << (heightPixels - z) << ' ' << std::flush;
				std::vector<glm::dvec3> row;
				for (size_t y = 0; y < widthPixels; y++) {
					glm::dvec3 finalCol (0.0, 0.0, 0.0); 
					

					std::random_device rd;   // Obtain a random seed
					std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
					std::uniform_real_distribution<> dis(0.0, 1.0);

					for (size_t N = 0; N < samples; N++) {
						

						double u_offset = dis(gen);
						double v_offset = dis(gen);

						double u = ((y + u_offset) / widthPixels) * imagePlaneWidth - imagePlaneWidth / 2;
						double v = (1.0 - (z + v_offset) / heightPixels) * imagePlaneHeight - imagePlaneHeight / 2;

						Ray firstRay = shootStartRay(glm::dvec3(-1.0, 0.0, 0.0), u, v); // Have to adjust eye pos based on camera position
						shootNextRay(firstRay);

						glm::dvec3 pixelColor = calcColor(firstRay);

						//std::cout << firstRay.radiance.x << " " << firstRay.radiance.y << " " << firstRay.radiance.z << "\n";
						
						finalCol += firstRay.radiance;
					}
					finalCol /= samples;
					row.push_back(finalCol); //TODO: replace with generateRay
					//row.push_back((y % 2 == 0 ? glm::dvec3(0, 0, 0) : glm::dvec3(255, 255, 255))); //TODO: replace with generateRay
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
					imageBuffer[index + 0] = static_cast<unsigned char>(sqrt(frameBuffer[y][x][0] / largest) * 255);  // R
					imageBuffer[index + 1] = static_cast<unsigned char>(sqrt(frameBuffer[y][x][1] / largest) * 255);  // G
					imageBuffer[index + 2] = static_cast<unsigned char>(sqrt(frameBuffer[y][x][2] / largest) * 255);  // B
				}
			}
			
			// Write the buffer to a PNG file
			stbi_write_png("render.png", widthPixels, heightPixels, 3, imageBuffer.data(), widthPixels * 3);
		}

	private:
		

	};



