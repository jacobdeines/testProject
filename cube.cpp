#include "geo.h"
#include "math.h"

using namespace std;

CubeClass::CubeClass(int size, Location location):
	size(size),
	location(location)
{
	updateCube();

	//Set edges (constant)
	edges[0] = {0, 1};
	edges[1] = {0, 2};
	edges[2] = {0, 4};
	edges[3] = {1, 3};
	edges[4] = {1, 5};
	edges[5] = {2, 3};
	edges[6] = {2, 6};
	edges[7] = {3, 7};
	edges[8] = {4, 5};
	edges[9] = {4, 6};
	edges[10] = {5, 7};
	edges[11] = {6, 7};
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

void CubeClass::Translate(Point move)
{
	location.position.x = location.position.x + move.x;
	location.position.y = location.position.y + move.y;
	location.position.z = location.position.z + move.z;

	updateCube();
}

void CubeClass::updateCube()
{
	//Set vertices
	vertices[0] = location.position;
	vertices[1] = {location.position.x + size, location.position.y, location.position.z};
	vertices[2] = {location.position.x, location.position.y + size, location.position.z};
	vertices[3] = {location.position.x + size, location.position.y + size, location.position.z};
	vertices[4] = {location.position.x, location.position.y, location.position.z + size};
	vertices[5] = {location.position.x + size, location.position.y, location.position.z + size};
	vertices[6] = {location.position.x, location.position.y + size, location.position.z + size};
	vertices[7] = {location.position.x + size, location.position.y + size, location.position.z + size};
}