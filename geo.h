#ifndef GEO_H
#define GEO_H

typedef struct
{
	int x;
	int y;
	int z;	//Optional, not used if in 2D on the screen
} Point;

typedef struct
{
	int pointA;
	int pointB;
} Line;

//Euler angles in 3D space
typedef struct
{
	double roll;
	double pitch;
	double yaw;
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

		void Translate(Point move);
		void SetPosition(Point pos);
		void SetPositionX(int pos);
		void SetPositionY(int pos);
		void SetPositionZ(int pos);

	private:
		int size;
		Location location;
		Point vertices[8];
		Line edges[12];

		void updateCube();
};

#endif