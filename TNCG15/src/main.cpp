// TNCG15.cpp : Defines the entry point for the application.
//

#include "TNCG15.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Camera.h"
#include "Sphere.h"
#include "Tetrahedron.h"
#include <vector>



int main()
{
	const int imageWidth = 200;
	const int imageHeight = 200;
	const glm::dvec3 cam_pos(0, 0, 0);
	const glm::dvec3 cam_up(0,0,1);
	const glm::dvec3 cam_forward(1,0,0);
	const double cam_fov = 80;
	 //TODO: fixa så att aspectRatio och fov beräknas i constructorn istället
	std::vector<Polygon*> lights;
	std::vector<Polygon*> scene;
	std::vector<Polygon*> sceneObjects;

	//Sphere* test = new Sphere(glm::dvec3(10, 0, 0), 1.0, glm::dvec3(0.2, 1.0, 0.2), 2, 0);
	Sphere* sphere2 = new Sphere(glm::dvec3(6, 1, 1), 1.0, glm::dvec3(0.5, 0.2, 0.8), 2, 0.5); 
	scene.push_back(sphere2);
	sceneObjects.push_back(sphere2);

	Sphere* sphere1 = new Sphere(glm::dvec3(10, 0, 0), 2.0, glm::dvec3(0.2, 1.0, 0.2), 2, 0.5);
	scene.push_back(sphere1);
	sceneObjects.push_back(sphere1);
	//scene.push_back(sphere2);
	//sceneObjects.push_back(sphere2);
	//Tetrahedron* tetra = new Tetrahedron(glm::dvec3(5, -2, -2), glm::dvec3(3, -2, -5), glm::dvec3(7, -5, -5), glm::dvec3(7, 1, -5), glm::dvec3(0.2, 1, 0.2), 2, 0);
	/*sceneObjects.push_back(sphere2);
	scene.push_back(sphere2);
	sceneObjects.push_back(sphere1);
	scene.push_back(sphere1);*/
	//scene.push_back(test);
	//sceneObjects.push_back(test);
	
	
	//Triangle* test = new Triangle(glm::dvec3(10, 4, 5), glm::dvec3(10, 2, -3), glm::dvec3(10, 5, -3),  glm::dvec3(0, 0, 0), false);
	//scene.push_back(test);
	
	//Lights ----------------------------------------------------------------------------------------
	Rectangle* light1 = new Rectangle(glm::dvec3(8, -2, 4.99), glm::dvec3(8, 2, 4.99), glm::dvec3(11, 2, 4.99), glm::dvec3(11, -2, 4.99), glm::dvec3(1.0, 1.0, 1.0), 0, 0, true);
	lights.push_back(light1);
	scene.push_back(light1);
	//-----------------------------------------------------------------------------------------------

	// Floor ----------------------------------------------------------------------------------------
	Rectangle* floor1 = new Rectangle(glm::dvec3(10,6,-5), glm::dvec3(0,6,-5), glm::dvec3(0,-6,-5), glm::dvec3(10,-6,-5),glm::dvec3(1.0, 0.2, 0.3), 2, 0.5,true);
	Triangle* floor2 = new Triangle(glm::dvec3(0, 6, -5), glm::dvec3(-3, 0, -5), glm::dvec3(0, -6, -5), glm::dvec3(1.0, 0.2, 0.3), 2, 0.5, true);
	Triangle* floor3 = new Triangle(glm::dvec3(10, 6, -5), glm::dvec3(10, -6, -5), glm::dvec3(13, 0, -5), glm::dvec3(1.0, 0.2, 0.3), 2, 0.5, true);
	scene.push_back(floor1);
	scene.push_back(floor2);
	scene.push_back(floor3);
	//-----------------------------------------------------------------------------------------------

	// Roof -----------------------------------------------------------------------------------------
	Rectangle* roof1 = new Rectangle(glm::dvec3(0, 6, 5), glm::dvec3(10, 6, 5), glm::dvec3(10, -6, 5), glm::dvec3(0, -6, 5), glm::dvec3(1.0, 1.0, 1.0), 2, 0.5, true);
	Triangle* roof2 = new Triangle(glm::dvec3(0, -6, 5), glm::dvec3(-3, 0, 5), glm::dvec3(0, 6, 5), glm::dvec3(1.0, 1.0, 1.0), 2, 0.5, true);
	Triangle* roof3 = new Triangle(glm::dvec3(10, -6, 5), glm::dvec3(10, 6, 5), glm::dvec3(13, 0, 5), glm::dvec3(1.0, 1.0, 1.0), 2, 0.5, true);
	scene.push_back(roof1);
	scene.push_back(roof2);
	scene.push_back(roof3);
	//-----------------------------------------------------------------------------------------------

	// Walls ----------------------------------------------------------------------------------------
	//bottom
	Rectangle* wall1 = new Rectangle(glm::dvec3(-3, 0, 5), glm::dvec3(0, -6, 5), glm::dvec3(0, -6, -5), glm::dvec3(-3, 0, -5), glm::dvec3(1.0, 1.0, 1.0), 2, 0.5, true);
	Rectangle* wall2 = new Rectangle(glm::dvec3(0, -6, 5), glm::dvec3(10, -6, 5), glm::dvec3(10, -6, -5), glm::dvec3(0, -6, -5), glm::dvec3(0.1, 1.0, 1.0), 2, 0.5, true);
	Rectangle* wall3 = new Rectangle(glm::dvec3(10, -6, 5), glm::dvec3(13, 0, 5), glm::dvec3(13, 0, -5), glm::dvec3(10, -6, -5), glm::dvec3(1.0, 1.0, 1.0), 2, 0.5, true); //Målvägg höger //looking at
	scene.push_back(wall1);
	scene.push_back(wall2);
	scene.push_back(wall3);

	//top
	Rectangle* wall4 = new Rectangle(glm::dvec3(-3, 0, 5), glm::dvec3(-3, 0, -5), glm::dvec3(0, 6, -5), glm::dvec3(0, 6, 5), glm::dvec3(1.0, 1.0, 1.0), 2, 0.5, true);
	Rectangle* wall5 = new Rectangle(glm::dvec3(0, 6, 5), glm::dvec3(0, 6, -5), glm::dvec3(10, 6, -5), glm::dvec3(10, 6, 5), glm::dvec3(0.4, 1.0, 1.0), 2, 0.5, true);
	Rectangle* wall6 = new Rectangle(glm::dvec3(10, 6, 5), glm::dvec3(10, 6, -5), glm::dvec3(13, 0, -5), glm::dvec3(13, 0, 5), glm::dvec3(1.0, 1.0, 1.0), 2, 0.5, true); //Målvägg vänster //looking at
	scene.push_back(wall4);
	scene.push_back(wall5);
	scene.push_back(wall6);
	
	//-----------------------------------------------------------------------------------------------
	//scene.push_back(light);
	Camera mainCamera(cam_pos, cam_forward, cam_up, cam_fov, imageWidth, imageHeight, scene, sceneObjects, lights);
	
	mainCamera.render();

	return 0;
}
