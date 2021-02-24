#include "geo.h"
#include "math.h"

using namespace std;

CubeClass::CubeClass(int size, Location location):
	size(size),
	location(location)
{
	updateCube();
}

CubeClass::~CubeClass()
{
	//Do nothing
}

int CubeClass::GetSize()
{
	return size;
}

Location CubeClass::GetLocation()
{
	return location;
}

Point *CubeClass::GetVertices()
{
	return vertices;
}

Line *CubeClass::GetEdges()
{
	return edges;
}

void CubeClass::updateCube()
{
	//Set vertices
	vertices[0] = location.position;
	vertices[1] = {location.position.x + size, location.position.y, location.position.z};
	vertices[2] = {location.position.x, location.position.y, location.position.z + size};
	vertices[3] = {location.position.x + size, location.position.y, location.position.z + size};
	//Eventually change these next 4 to their correct 3D versions... just a square for now
	vertices[4] = {location.position.x, location.position.y + size, location.position.z};
	vertices[5] = {location.position.x + size, location.position.y + size, location.position.z};
	vertices[6] = {location.position.x, location.position.y + size, location.position.z + size};
	vertices[7] = {location.position.x + size, location.position.y + size, location.position.z + size};

	//Set edges
	edges[0] = {vertices[0], vertices[1]};
	edges[1] = {vertices[0], vertices[2]};
	edges[2] = {vertices[0], vertices[4]};
	edges[3] = {vertices[1], vertices[3]};
	edges[4] = {vertices[1], vertices[5]};
	edges[5] = {vertices[2], vertices[3]};
	edges[6] = {vertices[2], vertices[6]};
	edges[7] = {vertices[3], vertices[7]};
	edges[8] = {vertices[4], vertices[5]};
	edges[9] = {vertices[4], vertices[6]};
	edges[10] = {vertices[5], vertices[7]};
	edges[11] = {vertices[6], vertices[7]};
}