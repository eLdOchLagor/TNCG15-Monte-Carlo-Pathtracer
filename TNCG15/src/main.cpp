// TNCG15.cpp : Defines the entry point for the application.
//

#include "TNCG15.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Camera.h"
#include <vector>



int main()
{
	const int imageWidth = 800;
	const int imageHeight = 800;
	const glm::vec3 cam_pos(0, 0, 0);
	const glm::vec3 cam_up(0,0,1);
	const glm::vec3 cam_forward(1,0,0);
	const float cam_fov = 80;
	 //TODO: fixa så att aspectRatio och fov beräknas i constructorn istället

	std::vector<Polygon*> scene;

	//Triangle* test = new Triangle(glm::vec3(10, 4, 5), glm::vec3(10, 2, -3), glm::vec3(10, 5, -3),  glm::vec3(0, 0, 0), false);
	//scene.push_back(test);
	
	// Floor ----------------------------------------------------------------------------------------
	Rectangle* floor1 = new Rectangle(glm::vec3(10,6,-5), glm::vec3(0,6,-5), glm::vec3(0,-6,-5), glm::vec3(10,-6,-5),glm::vec3(1.0, 1.0, 1.0));
	Triangle* floor2 = new Triangle(glm::vec3(0, 6, -5), glm::vec3(-3, 0, -5), glm::vec3(0, -6, -5), glm::vec3(1.0, 1.0, 1.0));
	Triangle* floor3 = new Triangle(glm::vec3(10, 6, -5), glm::vec3(10, -6, -5), glm::vec3(13, 0, -5), glm::vec3(1.0, 1.0, 1.0));
	scene.push_back(floor1);
	scene.push_back(floor2);
	scene.push_back(floor3);
	//-----------------------------------------------------------------------------------------------

	// Roof -----------------------------------------------------------------------------------------
	Rectangle* roof1 = new Rectangle(glm::vec3(0, 6, 5), glm::vec3(10, 6, 5), glm::vec3(10, -6, 5), glm::vec3(0, -6, 5), glm::vec3(1.0, 1.0, 1.0));
	Triangle* roof2 = new Triangle(glm::vec3(0, -6, 5), glm::vec3(-3, 0, 5), glm::vec3(0, 6, 5), glm::vec3(1.0, 1.0, 1.0));
	Triangle* roof3 = new Triangle(glm::vec3(10, -6, 5), glm::vec3(10, 6, 5), glm::vec3(13, 0, 5), glm::vec3(1.0, 1.0, 1.0));
	scene.push_back(roof1);
	scene.push_back(roof2);
	scene.push_back(roof3);
	//-----------------------------------------------------------------------------------------------

	// Walls ----------------------------------------------------------------------------------------
	//bottom
	Rectangle* wall1 = new Rectangle(glm::vec3(-3, 0, 5), glm::vec3(0, -6, 5), glm::vec3(0, -6, -5), glm::vec3(-3, 0, -5), glm::vec3(1.0, 1.0, 1.0));
	Rectangle* wall2 = new Rectangle(glm::vec3(0, -6, 5), glm::vec3(10, -6, 5), glm::vec3(10, -6, -5), glm::vec3(0, -6, -5), glm::vec3(1.0, 1.0, 1.0));
	Rectangle* wall3 = new Rectangle(glm::vec3(10, -6, 5), glm::vec3(13, 0, 5), glm::vec3(13, 0, -5), glm::vec3(10, -6, -5), glm::vec3(1.0, 1.0, 1.0)); //Målvägg höger //looking at
	scene.push_back(wall1);
	scene.push_back(wall2);
	scene.push_back(wall3);

	//top
	Rectangle* wall4 = new Rectangle(glm::vec3(-3, 0, 5), glm::vec3(-3, 0, -5), glm::vec3(0, 6, -5), glm::vec3(0, 6, 5), glm::vec3(1.0, 1.0, 1.0));
	Rectangle* wall5 = new Rectangle(glm::vec3(0, 6, 5), glm::vec3(0, 6, -5), glm::vec3(10, 6, -5), glm::vec3(10, 6, 5), glm::vec3(1.0, 1.0, 1.0));
	Rectangle* wall6 = new Rectangle(glm::vec3(10, 6, 5), glm::vec3(10, 6, -5), glm::vec3(13, 0, -5), glm::vec3(13, 0, 5), glm::vec3(1.0, 1.0, 1.0)); //Målvägg vänster //looking at
	scene.push_back(wall4);
	scene.push_back(wall5);
	scene.push_back(wall6);
	//-----------------------------------------------------------------------------------------------

	Camera mainCamera(cam_pos, cam_forward, cam_up, cam_fov, (float)imageWidth / imageHeight, scene);
	
	mainCamera.render(scene, imageHeight, imageHeight);

	return 0;
}
