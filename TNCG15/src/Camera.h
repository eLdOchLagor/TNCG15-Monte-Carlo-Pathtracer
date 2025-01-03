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
#include <chrono>
#include "Tetrahedron.h"
#include <algorithm>
#include "Photon.h"
#include "Sphere.h"
#include "kdtree.hpp"
#include <cstdlib>
#include "kdtree_homemade.h"

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

		//TODO: Skapa ett KD-Tree för de olika typerna av photonerna. 
		std::vector<Photon*> globalPhotons;
		std::vector<Photon*> shadowPhotons;
		Kdtree::KdNodeVector causticNodes;
		std::vector<kdtree_homemade::kd_Node*> caustics_test;

		struct Caustic {
			std::vector<double> coordinates; // x, y, z
			glm::dvec3 flux;         // associated flux
		};

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

		double generateRandomValue(double min = 0,double max = 1) {
			static std::random_device rd;   // Obtain a random seed
			static std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
			std::uniform_real_distribution<> dis(min, max);

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

		Ray* perfectRefraction(Ray* r, double n1, double n2) {
			double R = n1 / n2;

			if (n1 == 1.5) // If exiting object, flip normal
			{
				r->hit_surface->normal = -r->hit_surface->normal;
			}
			
			double cos_theta_i = glm::dot(r->direction, r->hit_surface->normal); // Cosine of the incident angle
			double cos_theta_t = sqrt(1.0 - R * R * (1.0 - cos_theta_i * cos_theta_i));

			// Calculate the refracted direction using Snell's law
			glm::dvec3 d_refr = R * r->direction + (R * cos_theta_i - cos_theta_t) * r->hit_surface->normal;

			glm::dvec3 startPoint = r->end_point + d_refr * epsilon;
			glm::dvec3 importance = r->radiance;
			Ray* newRay = new Ray(startPoint, d_refr, importance);
			newRay->currentRefractiveMedium = n2;
			newRay->depth = r->depth + 1;
			r->next_ray = newRay;
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
		glm::dvec3 calculate_caustic_flux(const glm::dvec3& M,const glm::dvec3& C,const double& r_0) {
			glm::dvec3 d = M - C;
			double G_m = glm::dot(glm::dvec3(0, 0, -1), (-glm::normalize(d)))/(glm::dot(d,d));
			double A_s = M_PI*r_0*r_0 * G_m;
			glm::dvec3 flux_emitted = glm::dvec3(8 *G_m* A_s);
			
			return flux_emitted;
		}

		glm::dvec3 caluclate_point_on_sphere(const glm::dvec3& M,const glm::dvec3& C,const double& r_0) {


			glm::dvec3 z_l = glm::normalize(M - C);
			glm::dvec3 y_l = glm::normalize(glm::cross(z_l, glm::dvec3(1.0, 0.0, 0.0)));
			glm::dvec3 x_l = glm::normalize(glm::cross(y_l, z_l));
			
			double randomValueAz = generateRandomValue();
			double randomValueR = sqrt(generateRandomValue());
			randomValueAz *= 2 * M_PI;
			randomValueR *= r_0;

		
			double a = randomValueR * cos(randomValueAz);
			double b = randomValueR * sin(randomValueAz);

			

			return glm::dvec3(C + a * x_l + b * y_l);
		}

		void initializeCausticPhotons() {
			const int N = 100000;

			glm::dvec3 e1 = lights[0]->verticies[1] - lights[0]->verticies[0];
			glm::dvec3 e2 = lights[0]->verticies[3] - lights[0]->verticies[0];

			
			Sphere* sphere;

			for (Polygon* obj : objects)
			{
				if (typeid(*obj) == typeid(Sphere) && obj->surfaceID == 3) {
					sphere = dynamic_cast<Sphere*>(obj);
				}
			}
			glm::dvec3 C = sphere->center;
			double r_0 = sphere->radius;
			//std::cout << C.x << "\n\n";
			
			glm::dvec3 M = glm::dvec3(lights[0]->verticies[0]) + 0.5 * e1 + 0.5 * e2;
			glm::dvec3 flux_causticPhoton = glm::dvec3(100)*calculate_caustic_flux(M, C, r_0)/static_cast<double>(N); // TODO: Dubbelkolla N
			for (size_t n = 0; n < N; n++) {
				double s = generateRandomValue();
				double t = generateRandomValue();
				
				glm::dvec3 x_s = glm::dvec3(lights[0]->verticies[0]) + s * e1 + t * e2;
				
				
				glm::dvec3 x_e = caluclate_point_on_sphere(x_s, C, r_0);
				
				
				Ray* causticPhotonRay = new Ray(M, x_e - M, flux_causticPhoton);
				
				shootCausticPhoton(*causticPhotonRay);
				while (causticPhotonRay->next_ray != nullptr) {
					causticPhotonRay = causticPhotonRay->next_ray;
				}
				
				Caustic point;
				point.coordinates.push_back(causticPhotonRay->end_point.x);
				point.coordinates.push_back(causticPhotonRay->end_point.y);
				point.coordinates.push_back(causticPhotonRay->end_point.z);
				point.flux = flux_causticPhoton;


				causticNodes.push_back(Kdtree::KdNode(point.coordinates, point.flux));
				glm::dvec3 points = glm::dvec3(point.coordinates[0], point.coordinates[1], point.coordinates[2]);
				caustics_test.push_back(new kdtree_homemade::kd_Node(points, point.flux));

				/*for (auto& temp : caustics_test) {
					if (temp->position.z > -5) {
						std::cout << temp->position.x << ", " << temp->position.y << ", " << temp->position.z << "\n\n";
					}
					
				}*/
				//std::cout << flux_causticPhoton.x << ", " << flux_causticPhoton.y << ", " << flux_causticPhoton.z << "\n\n";

				



			}
			//std::cout << std::size(caustics_test) << "\n\n";
			//std::cout << causticNodes.size();
			//std::cout << std::size(globalPhotons);
		}

		void shootCausticPhoton(Ray& r) {
			Ray* previousRay = &r;

			while (true) {
				double closestT = -1.0;
				Polygon* closestSurface = nullptr;
				std::vector<std::pair<double, Polygon*>> hitSurfaces;

				for (Polygon* obj : objects)
				{
					if (typeid(*obj) == typeid(Tetrahedron)) {
						for (Triangle* tri : dynamic_cast<Tetrahedron*>(obj)->triangles) {
							double t = tri->surfaceIntersectionTest(*previousRay);
							if (t > epsilon && (closestT < 0.0 || t < closestT)) {
								closestT = t;
								closestSurface = tri;

							}

						}
					}
					else if (typeid(*obj) == typeid(Rectangle)) {
						for (Triangle* tri : dynamic_cast<Rectangle*>(obj)->triangles) {
							double t = tri->surfaceIntersectionTest(*previousRay);
							if (t > epsilon && (closestT < 0.0 || t < closestT)) {
								closestT = t;
								closestSurface = tri;
							}


						}
					}
					else {
						double t = obj->surfaceIntersectionTest(*previousRay);
						if (t > epsilon && (closestT < 0.0 || t < closestT)) {
							closestT = t;
							closestSurface = obj;
						}
					}
				}
				
				previousRay->hit_surface = closestSurface;
				previousRay->end_point = previousRay->start_point + closestT * previousRay->direction;
				
				if (previousRay->hit_surface->surfaceID == 0) { // If ray hits lightsource
					previousRay->radiance = lights[0]->color;
					
					break;
				}

				if (previousRay->hit_surface->surfaceID == 1) { // If ray hits a mirror
					previousRay = perfectReflection(previousRay);
				}
				else if (previousRay->hit_surface->surfaceID == 2) { // If ray hits a diffuse reflector
					
					
					break;
				}
				else if (previousRay->hit_surface->surfaceID == 3) // If ray hits a transparent object
				{
					
					double R0 = (1 - 1.5) / (1 + 1.5) * (1 - 1.5) / (1 + 1.5); // Works both ways with n1, n2
					//double R = R0 + (1 - R0) * pow((1 - glm::dot(previousRay->direction, previousRay->hit_surface->normal)), 5); // Chance to reflect, derived from Schlick's formula

					if (previousRay->currentRefractiveMedium == 1.0) { // Hitting object on the outside
						
						double R = R0 + (1 - R0) * pow((1 - (glm::dot(previousRay->direction, -previousRay->hit_surface->normal))), 5);
						if (generateRandomValue() < R) // Reflect
						{
							previousRay = perfectReflection(previousRay);
						}
						else // Refract into object
						{
							
							previousRay = perfectRefraction(previousRay, 1, 1.5);
							
						}
					}
					else if (previousRay->currentRefractiveMedium == 1.5) // Inside glass object
					{
						
						double R = R0 + (1 - R0) * pow((1 - (glm::dot(previousRay->direction, previousRay->hit_surface->normal))), 5);
						//std::cout << "Inside object";
						double angle = acos(glm::dot(previousRay->direction, previousRay->hit_surface->normal));

						if (1.5 * sin(angle) > 1.0) // If total internal reflection, reflect
						{
							
							previousRay = perfectReflection(previousRay);
							previousRay->currentRefractiveMedium = 1.5;
						}
						else // Perform russian roulette to determine whether to reflect or refract out of the object
						{

							if (generateRandomValue() < R) { // Reflect
								
								previousRay = perfectReflection(previousRay);
								previousRay->currentRefractiveMedium = 1.5;
							}
							else // Refract out of the object
							{
								
								
								previousRay = perfectRefraction(previousRay, 1.5, 1);
							}
						}
					}
				}
			}

		}

		void shootNextRay(Ray& firstRay, Kdtree::KdTree& KDtree) {
			
			Ray* previousRay = &firstRay;
			while (true) {//previousRay->depth < 5000 tog bort att vi dödar ray paths då det inte funkar med reflective/refractive ytor
				double closestT = -1.0;
				Polygon* closestSurface = nullptr;
				std::vector<std::pair<double, Polygon*>> hitSurfaces;

				for (Polygon* obj : objects)
				{
					if (typeid(*obj) == typeid(Tetrahedron)) {
						for (Triangle* tri : dynamic_cast<Tetrahedron*>(obj)->triangles) {
							double t = tri->surfaceIntersectionTest(*previousRay);
							if (t > epsilon && (closestT < 0.0 || t < closestT)) {
								closestT = t;
								closestSurface = tri;

							}
							
						}
					}
					else if(typeid(*obj) == typeid(Rectangle)) {
						for (Triangle* tri : dynamic_cast<Rectangle*>(obj)->triangles) {
							double t = tri->surfaceIntersectionTest(*previousRay);
							if (t > epsilon && (closestT < 0.0 || t < closestT)) {
								closestT = t;
								closestSurface = tri;
							}
							
							
						}
					}
					else {
						double t = obj->surfaceIntersectionTest(*previousRay);
						if (t > epsilon && (closestT < 0.0 || t < closestT)) {
							closestT = t;
							closestSurface = obj;
						}
						
					}
					
					
					
				}
				//TODO: just nu så skickar vi bara tillbaka 1 av två t för sphere och tetrahydron, vilken inte funkar för shadowPhotons. Måste fixas! :: Har fixats, ska testas.
				
				//TODO: Måste fixa causticRays samt GlobalRays. Gör Global först, då de nästan är klara.
				//För att göra causticRays måste vi fixa area intersection med spheres. 

				//Anpassa pathtracern till att använda sig av våran photon mapper.
				
				previousRay->hit_surface = closestSurface;
				previousRay->end_point = previousRay->start_point + closestT * previousRay->direction;
				//std::cout << closestT << "\n";

				if (previousRay->hit_surface->surfaceID == 0) { // If ray hits lightsource
					previousRay->radiance = lights[0]->color;
					break;
				}

				if (previousRay->hit_surface->surfaceID == 1) { // If ray hits a mirror
					previousRay = perfectReflection(previousRay);
				}
				else if (previousRay->hit_surface->surfaceID == 2) { // If ray hits a diffuse reflector
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
				else if (previousRay->hit_surface->surfaceID == 3) // If ray hits a transparent object
				{	
					double R0 = (1 - 1.5) / (1 + 1.5) * (1 - 1.5) / (1 + 1.5); // Works both ways with n1, n2
					//double R = R0 + (1 - R0) * pow((1 - glm::dot(previousRay->direction, previousRay->hit_surface->normal)), 5); // Chance to reflect, derived from Schlick's formula

					if (previousRay->currentRefractiveMedium == 1.0){ // Hitting object on the outside
						
						double R = R0 + (1 - R0) * pow((1 - (glm::dot(previousRay->direction, -previousRay->hit_surface->normal))), 5);
						if (generateRandomValue() < R) // Reflect
						{
							previousRay = perfectReflection(previousRay);
						}
						else // Refract into object
						{
							previousRay = perfectRefraction(previousRay, 1, 1.5);
						}
					}
					else if (previousRay->currentRefractiveMedium == 1.5) // Inside glass object
					{
						double R = R0 + (1 - R0) * pow((1 - (glm::dot(previousRay->direction, previousRay->hit_surface->normal))), 5);
						//std::cout << "Inside object";
						double angle = acos(glm::dot(previousRay->direction, previousRay->hit_surface->normal));

						if (1.5 * sin(angle) > 1.0) // If total internal reflection, reflect
						{
							previousRay = perfectReflection(previousRay);
							previousRay->currentRefractiveMedium = 1.5;
						}
						else // Perform russian roulette to determine whether to reflect or refract out of the object
						{
							
							if (generateRandomValue() < R) { // Reflect

								previousRay = perfectReflection(previousRay);
								previousRay->currentRefractiveMedium = 1.5;
							}
							else // Refract out of the object
							{
								previousRay = perfectRefraction(previousRay, 1.5, 1);
							}
						}
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
				if (rayPath->previous_ray->hit_surface->surfaceID == 1 || rayPath->previous_ray->hit_surface->surfaceID == 3) { // Mirror or transparent object
					
					rayPath->previous_ray->radiance = rayPath->radiance ;
					rayPath = rayPath->previous_ray;
					
				}
				else { // If diffuse object
					
					rayPath->previous_ray->radiance = glm::dvec3(rayPath->previous_ray->hit_surface->color.r * rayPath->radiance.r,
						rayPath->previous_ray->hit_surface->color.g * rayPath->radiance.g,
						rayPath->previous_ray->hit_surface->color.b * rayPath->radiance.b) +
						calculateDirectIllumination(rayPath->previous_ray) + calculateCausticContribution(rayPath,KDtree);

					
					rayPath = rayPath->previous_ray;
				}
			}
			
		}

		glm::dvec3 calculateCausticContribution(Ray* ray,Kdtree::KdTree& KDtree) {
			//std::vector<kdtree_homemade::kd_Node*> photons;
			//glm::dvec3 point = glm::dvec3(0);
			//KDtree.find_photons(photons, KDtree.root, point, 0.2);

			/*for (kdtree_homemade::kd_Node* temp : photons)
			{
				totalFlux += temp->flux;
				//std::cout << temp->flux.x << ", " << temp->flux.y << ", " << temp->flux.z << ",        " << totalFlux.x << ", " << totalFlux.y << ", " << totalFlux.z <<  "\n";
				//std::cout << point.x <<", "<<point.y<<", "<<point.z<< "      " << temp->position.x << ", " << temp->position.y << ", " << temp->position.z << "\n";
			}*/
			//if (photons.size() > 0) {
				//std::cout << "\n";
			//}
			double radius = 0.2;
			Kdtree::KdNodeVector result;
			std::vector<double> point(3);
			
			point[0] = ray->start_point.x;
			point[1] = ray->start_point.y;
			point[2] = ray->start_point.z;
			
			
			KDtree.range_nearest_neighbors(point, radius, &result);
			glm::dvec3 totalFlux = glm::dvec3(0.0);
			
			for (auto& temp : result) {
				totalFlux += temp.flux;
			}
			totalFlux /= (M_PI * radius * radius);
			
			return totalFlux;
		}

		glm::dvec3 calculateDirectIllumination(Ray* ray) {
			glm::dvec3 radiance(0.0, 0.0, 0.0);
			double N = 5;
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
				radiance *= A*lights[0]->color/M_PI ;
			}
			

			return radiance *= surfaceColor/(N); //surfaceColor *
		}
		bool isInShadow(const glm::dvec3& point, const glm::dvec3& lightPos, const Polygon* originSurface) {
			Ray* shadowRay = new Ray(point, glm::normalize(lightPos - point), glm::dvec3(0,0,0));

			for (Polygon* obj : sceneObjects) {
				if (obj == originSurface || obj->surfaceID == 3) // If shadowray hits starting object or transparent object, Ignore
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
		void print_nodes(const Kdtree::KdNodeVector& nodes) {
			size_t i, j;
			for (i = 0; i < nodes.size(); ++i) {
				if (i > 0)
					std::cout << " ";
				std::cout << "(";
				for (j = 0; j < nodes[i].point.size(); j++) {
					if (j > 0)
						std::cout << ",";
					std::cout << nodes[i].point[j];
				}
				std::cout << ")";
			}
			std::cout << std::endl;
		}
		void render() {
			
			std::vector<std::vector<glm::dvec3>> frameBuffer;
			
			//Create image-matrix from raytrace
			auto start = std::chrono::high_resolution_clock::now();
			initializeCausticPhotons();
			Kdtree::KdTree KDtree(&causticNodes);
			kdtree_homemade::kdTree_homemade KDtree_homemade = kdtree_homemade::kdTree_homemade(caustics_test);
			
			KDtree.set_distance(2);
			//std::cout << "Points in kd-tree\n";
			//print_nodes(tree.allnodes);
			int samples = 35;
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
						shootNextRay(*firstRay, KDtree);

						//std::cout << firstRay.radiance.x << " " << firstRay.radiance.y << " " << firstRay.radiance.z << "\n";
						
						finalCol += firstRay->radiance;
						firstRay->dealocateRay(*firstRay);
						
					}
					finalCol /= samples;
					row.push_back(finalCol); 
					//row.push_back((y % 2 == 0 ? glm::dvec3(0, 0, 0) : glm::dvec3(255, 255, 255))); //TODO: replace with generateRay
				}
				frameBuffer.push_back(row);
			}

			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> time_taken = end - start;
			std::cout << "\nTime taken to render : " << time_taken.count() << " sec \n";

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



