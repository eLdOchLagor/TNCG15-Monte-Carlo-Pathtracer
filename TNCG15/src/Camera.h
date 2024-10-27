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
		double epsilon = 1e-10;
		std::vector<Polygon*> lights; 

		std::vector<Polygon*> objects;
		std::vector<Polygon*> sceneObjects;

		Camera(glm::dvec3 pos, glm::dvec3 fwd, glm::dvec3 up, double fov, int width, int height, std::vector<Polygon*> obj, std::vector<Polygon*> sceObj, std::vector<Polygon*> lightObj) : position(pos), forward(fwd), fov(fov), widthPixels{ width }, heightPixels{ height }, objects(obj), sceneObjects{sceObj}, lights{lightObj} {
			aspectRatio = (double)width / height;
			right = glm::normalize(glm::cross(forward, up));
			trueUp = glm::cross(right, forward);
			imagePlaneHeight = 2.0f * tan(glm::radians(fov) / 2.0f);
			imagePlaneWidth = imagePlaneHeight * aspectRatio;
		}

		Ray* shootStartRay(glm::dvec3 eye, double u, double v) {
			//std::cout << right.y;
			glm::dvec3 direction = glm::normalize(forward + u * right + v * trueUp);
			Ray* firstRay = new Ray(eye, direction, glm::dvec3(1.0, 1.0, 1.0));

			return firstRay;
		}

		double generateRandomValue() {
			static std::random_device rd;   // Obtain a random seed
			static std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
			std::uniform_real_distribution<> dis(0.0, 1.0);

			return dis(rd);
		}

		Ray* perfectReflection(Ray* r) {
			glm::dvec3 d_o = glm::normalize(r->direction) - 2 * glm::dot(glm::normalize(r->direction), r->hit_surface->normal) * r->hit_surface->normal;
			glm::dvec3 startPoint = r->end_point;
			glm::dvec3 importance = r->radiance;
			Ray* newRay = new Ray(startPoint, d_o, importance);
			newRay->depth = r->depth+1;
			r->next_ray = newRay;
			//tempRay = previousRay->next_ray;
			newRay->previous_ray = r;
			return newRay;
		}

		Ray* diffuseReflection(Ray* r, double& randAz, double& randInc) {
			double x = cos(randAz) * sin(randInc);
			double y = sin(randAz) * sin(randInc);
			double z = cos(randInc);
			glm::dvec3 tangent, bitangent;
			glm::dvec3 normal = r->hit_surface->normal;

			tangent = glm::normalize(-r->direction + glm::dot(normal, r->direction) * normal);
			bitangent = glm::normalize(glm::cross(normal, tangent));

			glm::dvec3 worldDir = glm::normalize(normal * z + tangent * x + bitangent * y);

			glm::dvec3 startPoint = r->end_point;
			glm::dvec3 importance = r->radiance;
			Ray* newRay = new Ray(startPoint, worldDir, importance);
			newRay->depth = r->depth + 1;
			r->next_ray = newRay;
			//tempRay = previousRay->next_ray;
			newRay->previous_ray = r;
			i++;

			return newRay;
		}

		void shootNextRay(Ray& firstRay) {
			//std::cout << i << "\n";
			Ray* previousRay = &firstRay;
			while (true) {//previousRay->depth < 5000 tog bort att vi dödar ray paths då det inte funkar med reflective/refractive ytor
				double closestT = -1.0;
				Polygon* closestSurface = nullptr;
				
				for (Polygon* obj : objects)
				{
					double t = obj->surfaceIntersectionTest(*previousRay);
					if (t > epsilon && (closestT < 0.0 || t < closestT)) {
						closestT = t;
						closestSurface = obj;
					}

				}
				
				previousRay->hit_surface = closestSurface;
				previousRay->end_point = previousRay->start_point + closestT * previousRay->direction;
				//std::cout << closestT << "\n";

				if (previousRay->hit_surface->surfaceID == 0) {
					previousRay->radiance = glm::dvec3(1.0, 1.0, 1.0);
					break;
				}

				if (previousRay->hit_surface->surfaceID == 1) {
					//std::cout << "prickade en mirror";
					if (previousRay->hit_surface->reflectance != 1) { // If transparent object

						double R0 = 0.0;
						if (previousRay->currentRefractiveMedium == 1)
						{
							R0 = (1 - 1.5) / (1 + 1.5) * (1 - 1.5) / (1 + 1.5);
						}
						else
						{
							R0 = (1.5 - 1) / (1.5 + 1) * (1.5 - 1) / (1.5 + 1);
						}

						double R = R0 + (1 - R0) * pow((1 - cos(glm::dot(previousRay->direction, previousRay->hit_surface->normal))), 5);

						if (previousRay->currentRefractiveMedium != 1) // If in a object other than air
						{
							if (1/1.5 * sin(glm::dot(previousRay->direction, previousRay->hit_surface->normal)) < 1) // Reflect
							{
								
								previousRay = perfectReflection(previousRay);
								previousRay->radiance *= R;
								previousRay->currentRefractiveMedium = 1.5;
							}
							else // Perform russian roulette to determine whether to reflect or refract out of the object
							{
								double randomValue = generateRandomValue();

								if (randomValue < previousRay->hit_surface->reflectance) { // Reflect
									
									previousRay = perfectReflection(previousRay);
									previousRay->radiance *= R;
									previousRay->currentRefractiveMedium = 1.5;
								}
								else // Refract out of the object
								{
									
									double R1 = previousRay->currentRefractiveMedium == 1 ? 1 / 1.5 : 1.5;
									glm::dvec3 d_refr = R1 * previousRay->direction + previousRay->hit_surface->normal * (-R1 * glm::dot(previousRay->hit_surface->normal, previousRay->direction)) -
										sqrt(1 - R1 * R1 * (1 - glm::dot(previousRay->hit_surface->normal, previousRay->direction) * glm::dot(previousRay->hit_surface->normal, previousRay->direction)));
									glm::dvec3 startPoint = previousRay->end_point;

									glm::dvec3 importance = previousRay->radiance * (1- R1);
									Ray* newRay = new Ray(startPoint, d_refr, importance);
									newRay->currentRefractiveMedium = 1;
									newRay->depth = previousRay->depth + 1;
									previousRay->next_ray = newRay;
									//tempRay = previousRay->next_ray;
									newRay->previous_ray = previousRay;
									previousRay = newRay;
								}
							}
						}

						double randomValue = generateRandomValue();

						if (randomValue < previousRay->hit_surface->reflectance) { // Reflect
							
							previousRay = perfectReflection(previousRay);
							previousRay->radiance *= R;
						}
						else { // If russian roulette determines to refract
							
							//double R = previousRay->currentRefractiveMedium == 1 ? 1 / 1.5 : 1.5;
							glm::dvec3 d_refr = R * previousRay->direction + previousRay->hit_surface->normal * (-R * glm::dot(previousRay->hit_surface->normal, previousRay->direction)) -
								sqrt(1 - R * R * (1 - glm::dot(previousRay->hit_surface->normal, previousRay->direction) * glm::dot(previousRay->hit_surface->normal, previousRay->direction)));
							glm::dvec3 startPoint = previousRay->end_point;
							glm::dvec3 importance = previousRay->radiance * (1-R);
							Ray* newRay = new Ray(startPoint, d_refr, importance);
							newRay->depth = previousRay->depth + 1;
							previousRay->next_ray = newRay;
							//tempRay = previousRay->next_ray;
							newRay->previous_ray = previousRay;
							previousRay = newRay;
							//std::cout << newRay->start_point.r << " " << newRay->start_point.g << " " << newRay->start_point.b << "\n";
						}
					}
					else { // If perfect mirror

						previousRay = perfectReflection(previousRay);
					}
					
				}
				else if (previousRay->hit_surface->surfaceID == 2) {
					double Roh = previousRay->hit_surface->reflectance;

					double randomValue1 = generateRandomValue();
					double randomValue2 = generateRandomValue();

					double randInclination = acos(sqrt(1 - randomValue1));
					double randAzimuth = 2 * M_PI * randomValue2;
					double rr = randAzimuth / Roh;

					if (rr <= 2 * M_PI) {
						previousRay = diffuseReflection(previousRay, randAzimuth, randInclination);
					}
					//If we hit a diffuse surface and russian roulette determines that the ray is killed then thoust is the final if-statement determening 
					//the destiny of the radiance.
					else {
						break;
					}
				}
				else {



				}
			}

			// Traverse raypath from last ray to pixel
			Ray* rayPath = previousRay;
			if (rayPath->hit_surface->surfaceID != 0) {
				rayPath->radiance = calculateDirectIllumination(rayPath);
			}
			
			
			while (rayPath->previous_ray != nullptr) {
				if (rayPath->previous_ray->hit_surface->surfaceID == 1) { // Mirror or transparent object
					if (rayPath->previous_ray->hit_surface->reflectance == 1.0) // If mirror
					{
						rayPath->previous_ray->radiance = rayPath->radiance;
						rayPath = rayPath->previous_ray;
					}
					else // If transparent object
					{
						glm::dvec3 ratio = rayPath->radiance / rayPath->previous_ray->radiance;
						rayPath->previous_ray->radiance = glm::dvec3(ratio.r * rayPath->radiance.r,
							ratio.g * rayPath->radiance.g,
							ratio.b * rayPath->radiance.b);
						//rayPath->previous_ray->radiance = rayPath->radiance;

						rayPath = rayPath->previous_ray;
					}
					
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

		glm::dvec3 calculateDirectIllumination(Ray* ray) {
			glm::dvec3 radiance(0.0, 0.0, 0.0);
			double N = 15;
			glm::dvec3 surfaceColor = ray->hit_surface->color;
			for (Polygon* light : lights)
			{
				glm::dvec3 e1 = light->verticies[1] - light->verticies[0];
				glm::dvec3 e2 = light->verticies[3] - light->verticies[0];

				for (size_t n = 0; n < N; n++)
				{
					double s = generateRandomValue();
					double t = generateRandomValue();

					glm::dvec3 y = glm::dvec3(light->verticies[0]) + s * e1 + t * e2;
					glm::dvec3 di = y - glm::dvec3(ray->end_point);

					double cosx = glm::dot(ray->hit_surface->normal, glm::normalize(di));
					double cosy = glm::dot(-light->normal, glm::normalize(di));

					// Make sure that surfaces facing away from the lightsource dont give negative values, these values give wrong result
					cosx = std::max(0.0, cosx);
					cosy = std::max(0.0, cosy);

					const double epsilon = 1e-4;//1e-4
					if (!isInShadow(ray->end_point + epsilon * ray->hit_surface->normal, y, ray->hit_surface)) {
						double scalar_radiance = (cosx * cosy) / (glm::length(di) * glm::length(di));
						radiance += scalar_radiance;
					}

				}
				double A = glm::length(e1) * glm::length(e2);
				radiance *= A*10/M_PI ;
			}
			

			return radiance *= surfaceColor/(N); //surfaceColor *
		}
		bool isInShadow(const glm::dvec3& point, const glm::dvec3& lightPos, const Polygon* originSurface) {
			Ray* shadowRay = new Ray(point, glm::normalize(lightPos - point), glm::dvec3(0,0,0));

			for (Polygon* obj : sceneObjects) {
				if (obj == originSurface)
				{
					continue;
				}

				if (obj->surfaceIntersectionTest(*shadowRay) > 0.0) {
					delete shadowRay;
					shadowRay = nullptr;
					return true;  // Something is blocking the light
				}
			}
			delete shadowRay;
			shadowRay = nullptr;
			return false;  // Light is visible
		}

		void render() {
			std::vector<std::vector<glm::dvec3>> frameBuffer;
			
			//Create image-matrix from raytrace

			int samples = 50;
			for (size_t z = 0; z < heightPixels; z++) {
				std::clog << "\rScanlines remaining: " << (heightPixels - z) << ' ' << std::flush;
				std::vector<glm::dvec3> row;
				for (size_t y = 0; y < widthPixels; y++) {
					glm::dvec3 finalCol (0.0, 0.0, 0.0); 

					for (size_t N = 0; N < samples; N++) {
						double u_offset = generateRandomValue();
						double v_offset = generateRandomValue();

						double u = ((y + u_offset) / widthPixels) * imagePlaneWidth - imagePlaneWidth / 2;
						double v = (1.0 - (z + v_offset) / heightPixels) * imagePlaneHeight - imagePlaneHeight / 2;
						
						Ray* firstRay = shootStartRay(glm::dvec3(-1.0, 0.0, 0.0), u, v); // Have to adjust eye pos based on camera position
						shootNextRay(*firstRay);

						//std::cout << firstRay.radiance.x << " " << firstRay.radiance.y << " " << firstRay.radiance.z << "\n";
						
						finalCol += firstRay->radiance;
						firstRay->dealocateRay(*firstRay);
						
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



