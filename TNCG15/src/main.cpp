// TNCG15.cpp : Defines the entry point for the application.
//

#include "TNCG15.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Camera.h"
#include <fstream>
#include <vector>



int main()
{
	std::cout << "Hello CMake." << std::endl;
	const int imageWidth = 800;
	const int imageHeight = 800;
	const glm::vec3 cam_pos(0, 0, 0);
	const glm::vec3 cam_up(0,0,1);
	const glm::vec3 cam_forward(1,0,0);
	const float cam_fov = 3.14159f / 4.0f;
	Camera mainCamera(cam_pos, cam_forward, cam_up, cam_fov,(float)imageWidth/imageHeight); //TODO: fixa så att aspectRatio och fov beräknas i constructorn istället

	std::vector<Polygon*> scene;

	// Floor ----------------------------------------------------------------------------------------
	Rectangle* floor1 = new Rectangle(glm::vec3(0,6,-5), glm::vec3(10,6,-5), glm::vec3(10,-6,-5), glm::vec3(0,-6,-5),glm::vec3(0,255,0));
	Triangle* floor2 = new Triangle(glm::vec3(10, 6, -5), glm::vec3(13, 0, -5), glm::vec3(10, -6, -5), glm::vec3(0, 255, 0));
	Triangle* floor3 = new Triangle(glm::vec3(0, 6, -5), glm::vec3(-3, 0, -5), glm::vec3(0, -6, -5), glm::vec3(0, 255, 0));
	scene.push_back(floor1);
	scene.push_back(floor2);
	scene.push_back(floor3);
	//-----------------------------------------------------------------------------------------------

	// Roof -----------------------------------------------------------------------------------------
	Rectangle* roof1 = new Rectangle(glm::vec3(0, 6, 5), glm::vec3(10, 6, 5), glm::vec3(10, -6, 5), glm::vec3(0, -6, 5), glm::vec3(0, 255, 0));
	Triangle* roof2 = new Triangle(glm::vec3(10, 6, 5), glm::vec3(13, 0, 5), glm::vec3(10, -6, 5), glm::vec3(0, 255, 0));
	Triangle* roof3 = new Triangle(glm::vec3(0, 6, 5), glm::vec3(-3, 0, 5), glm::vec3(0, -6, 5), glm::vec3(0, 255,0));
	scene.push_back(roof1);
	scene.push_back(roof2);
	scene.push_back(roof3);
	//-----------------------------------------------------------------------------------------------

	// Walls ----------------------------------------------------------------------------------------
	//bottom
	Rectangle* wall1 = new Rectangle(glm::vec3(-3, 0, 5), glm::vec3(0, -6, 5), glm::vec3(0, -6, -5), glm::vec3(-3, 0, -5), glm::vec3(0, 255, 0)); 
	Rectangle* wall2 = new Rectangle(glm::vec3(0, -6, 5), glm::vec3(10, -6, 5), glm::vec3(10, -6, -5), glm::vec3(0, -6, -5), glm::vec3(255, 0, 0)); 
	Rectangle* wall3 = new Rectangle(glm::vec3(10, -6, 5), glm::vec3(13, 0, 5), glm::vec3(13, 0, -5), glm::vec3(10, -6, -5), glm::vec3(255, 255, 0)); //Målvägg höger //looking at
	scene.push_back(wall1);
	scene.push_back(wall2);
	scene.push_back(wall3);

	//top
	Rectangle* wall4 = new Rectangle(glm::vec3(-3, 0, 5), glm::vec3(-3, 0, -5), glm::vec3(0, 6, -5), glm::vec3(0, 6, 5), glm::vec3(0, 0, 0)); 
	Rectangle* wall5 = new Rectangle(glm::vec3(0, 6, 5), glm::vec3(0, 6, -5), glm::vec3(10, 6, -5), glm::vec3(10, 6, 5), glm::vec3(0, 255, 0)); 
	Rectangle* wall6 = new Rectangle(glm::vec3(10, 6, 5), glm::vec3(10, 6, -5), glm::vec3(13, 0, -5), glm::vec3(13, 0, 5), glm::vec3(255, 0, 255)); //Målvägg vänster //looking at
	scene.push_back(wall4);
	scene.push_back(wall5);
	scene.push_back(wall6);
	//-----------------------------------------------------------------------------------------------

	std::vector<std::vector<glm::vec3>> frameBuffer;
	
	// Create and open a text file
	std::ofstream OutputFile("render.ppm");

	// Setup PPM file settings
	OutputFile << "P3\n# This is a render!\n" << imageWidth << " " << imageHeight << "\n255\n";

	//Create image-matrix from raytrace
	for (size_t z = 0; z < imageHeight; z++) {
		std::vector<glm::vec3> row;
		for (size_t y = 0; y < imageWidth; y++) {
			
			float u = (2.0f * (y+0.5f)/imageWidth) - 1.0f;
			float v = 1.0f-(2.0f * (z + 0.5f)/imageHeight);

			Ray firstRay = mainCamera.shootStartRay(glm::vec3(-1.0, 0.0, 0.0), u, v);

			for (Polygon* temp : scene)
			{
				temp->surfaceIntersectionTest(firstRay);
			}

			//std::cout << firstRay.radiance.x << " " << firstRay.radiance.y << " " << firstRay.radiance.z << "\n";

			row.push_back(firstRay.radiance); //TODO: replace with generateRay
			//row.push_back((y % 2 == 0 ? glm::vec3(0, 0, 0) : glm::vec3(255, 255, 255))); //TODO: replace with generateRay
		}
		frameBuffer.push_back(row);
	}

	//Write imageFile from image-matrix
	for (size_t y = 0; y < imageHeight; y++) {
		for (size_t x = 0; x < imageWidth; x++) {

			OutputFile << frameBuffer[y][x][0] << " ";
			OutputFile << frameBuffer[y][x][1] << " ";
			OutputFile << frameBuffer[y][x][2] << " ";
		}
		OutputFile << "\n";
	}
	// Close the file
	OutputFile.close();

	return 0;
}
