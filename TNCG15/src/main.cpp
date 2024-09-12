// TNCG15.cpp : Defines the entry point for the application.
//

#include "TNCG15.h"
#include "Triangle.h"

using namespace std;

int main()
{
	cout << "Hello CMake." << endl;
	std::vector<double> init{ 10, 20, 30};
	Triangle::Triangle test{init};

	for (auto temp : test.get_Points()) {
		std::cout << temp;
	}
	return 0;
}
