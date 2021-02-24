#ifndef GEO_H
#define GEO_H

typedef struct
{
	float x;
	float y;
	float z;
} Point;

typedef struct
{
	Point pointA;
	Point pointB;
} Line;

//Euler angles in 3D space
typedef struct
{
	float alpha;
	float beta;
	float gamma;
} Angle;

typedef struct
{
	Point position;
	Angle orientation;
} Location;

class CubeClass
{
	public:
		CubeClass(float size, Location location);
		~CubeClass();
		float GetSize();
		Location GetLocation();

		Point *GetVertices();
		Line *GetEdges();

	private:
		float size;
		Location location;
};

#endif