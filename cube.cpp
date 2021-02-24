#include "geo.h"

using namespace std;

CubeClass::CubeClass(float size, Location location):
	size(size),
	location(location)
{
	//Do nothing
}

CubeClass::~CubeClass()
{
	//Do nothing
}

float CubeClass::GetSize()
{
	return this->size;
}

Location CubeClass::GetLocation()
{
	return this->location;
}

Point *CubeClass::GetVertices()
{
	//Do some calculating and return array of 8 points
}

Line *CubeClass::GetEdges()
{
	//Do some calculating and return array of 12 edges
}