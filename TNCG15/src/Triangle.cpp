
//TODO LUDWIG: lägg in din egen filepath på raden nedan, och kommentera bort min. Blir enkelt för oss båda att 
//gå mellan rätt filepaths.
#include "Triangle.h"
//#include "D:\Skola\RayTracing\TNCG15-project\TNCG15\out\build\TNCG15\Triangle.h"

namespace Triangle{

	Triangle::Triangle(std::vector<double> point) {
		points = point;
		points.push_back(1.0);
	}

}