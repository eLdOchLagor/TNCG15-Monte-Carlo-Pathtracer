
//TODO LUDWIG: l�gg in din egen filepath p� raden nedan, och kommentera bort min. Blir enkelt f�r oss b�da att 
//g� mellan r�tt filepaths.
#include "Triangle.h"
//#include "D:\Skola\RayTracing\TNCG15-project\TNCG15\out\build\TNCG15\Triangle.h"

namespace Triangle{

	Triangle::Triangle(std::vector<double> point) {
		points = point;
		points.push_back(1.0);
	}

}