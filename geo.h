#ifndef GEO_H
#define GEO_H

typedef struct
{
	int x;
	int y;
	int z;
} Point;

typedef struct
{
	Point pointA;
	Point pointB;
} Line;

//Euler angles in 3D space
typedef struct
{
	int roll;
	int pitch;
	int yaw;
} Angle;

typedef struct
{
	Point position;
	Angle orientation;
} Location;

class CubeClass
{
	public:
		CubeClass(int size, Location location);
		~CubeClass();
		int GetSize();
		Location GetLocation();

		Point *GetVertices();
		Line *GetEdges();

	private:
		int size;
		Location location;
		Point vertices[8];
		Line edges[12];

		void updateCube();
};

#endif