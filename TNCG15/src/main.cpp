// TNCG15.cpp : Defines the entry point for the application.
//

#include "TNCG15.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Camera.h"
#include <fstream>
#include <vector>

using namespace std;

int main()
{
	cout << "Hello CMake." << endl;
	const int imageWidth = 800;
	const int imageHeight = 800;
	Camera::Camera mainCamera(glm::vec3(0,0,0), glm::vec3(0,0,-1), glm::vec3(0,1,0), 3.14159f/4.0f,(float)imageWidth/imageHeight); //TODO: fixa så att aspectRatio och fov beräknas i constructorn istället
	
	// Floor ----------------------------------------------------------------------------------------
	Rectangle::Rectangle floor1(glm::vec3(0,6,-5), glm::vec3(10,6,-5), glm::vec3(10,-6,-5), glm::vec3(0,-6,-5),glm::vec3(255,0,0));
	Triangle::Triangle floor2(glm::vec3(10, 6, -5), glm::vec3(13, 0, -5), glm::vec3(10, -6, -5), glm::vec3(0, 255, 0));
	Triangle::Triangle floor3(glm::vec3(0, 6, -5), glm::vec3(-3, 0, -5), glm::vec3(0, -6, -5), glm::vec3(0, 0, 255));
	//-----------------------------------------------------------------------------------------------

	// Roof -----------------------------------------------------------------------------------------
	Rectangle::Rectangle roof1(glm::vec3(0, 6, 5), glm::vec3(10, 6, 5), glm::vec3(10, -6, 5), glm::vec3(0, -6, 5), glm::vec3(255, 0, 0));
	Triangle::Triangle roof2(glm::vec3(10, 6, 5), glm::vec3(13, 0, 5), glm::vec3(10, -6, 5), glm::vec3(0, 255, 0));
	Triangle::Triangle roof3(glm::vec3(0, 6, 5), glm::vec3(-3, 0, 5), glm::vec3(0, -6, 5), glm::vec3(0, 0, 255));
	//-----------------------------------------------------------------------------------------------

	// Walls ----------------------------------------------------------------------------------------
	//bottom
	Rectangle::Rectangle wall1(glm::vec3(-3, 0, 5), glm::vec3(-3, 0, -5), glm::vec3(0, -6, -5), glm::vec3(0, -6, 5), glm::vec3(255, 255, 0));
	Rectangle::Rectangle wall2(glm::vec3(0, -6, 5), glm::vec3(0, -6, -5), glm::vec3(10, -6, -5), glm::vec3(10, -6, 5), glm::vec3(255, 255, 0));
	Rectangle::Rectangle wall3(glm::vec3(10, 0, 5), glm::vec3(10, 0, -5), glm::vec3(13, -6, 0), glm::vec3(13, -6, 0), glm::vec3(255, 255, 0));
	//top
	Rectangle::Rectangle wall4(glm::vec3(-3, 5, 5), glm::vec3(0, 6, 5), glm::vec3(0, 6, -5), glm::vec3(-3, 0, -5), glm::vec3(255, 0, 255));
	Rectangle::Rectangle wall5(glm::vec3(0, 6, 5), glm::vec3(10, 6, 5), glm::vec3(10, 6, -5), glm::vec3(0, 6, -5), glm::vec3(255, 0, 255));
	Rectangle::Rectangle wall4(glm::vec3(10, 6, 5), glm::vec3(13, 0, 5), glm::vec3(13, 0, -5), glm::vec3(10, 6, -5), glm::vec3(255, 0, 255));
	//-----------------------------------------------------------------------------------------------

	vector<vector<glm::vec3>> frameBuffer;
	
	// Create and open a text file
	ofstream OutputFile("render.ppm");

	// Setup PPM file settings
	OutputFile << "P3\n# This is a render!\n" << imageWidth << " " << imageHeight << "\n255\n";

	//Create image-matrix from raytrace
	for (size_t y = 0; y < imageHeight; y++) {
		vector<glm::vec3> row;
		for (size_t x = 0; x < imageWidth; x++) {
			
			float u = (2.0f * (x+0.5f)/imageWidth) - 1.0f;
			float v = 1.0f-(2.0f * (y + 0.5f)/imageHeight);

			row.push_back((y % 2 == 0 ? glm::vec3(0, 0, 0) : glm::vec3(255, 255, 255))); //TODO: replace with generateRay
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
