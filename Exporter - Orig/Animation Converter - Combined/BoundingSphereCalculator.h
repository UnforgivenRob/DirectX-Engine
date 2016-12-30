#ifndef BOUNDINGSPHERECALCULATOR_H
#define BOUNDINGSPHERECALCULATOR_H

#include "Vert_Structs.h"
#include <MathEngine.h>

struct Sphere
{
	Vect center;
	float radius;
};

class BoundingSphereCalculator
{
public:
	static void CalculateSphere(Sphere& sphereIn, Vert_Final* verts, int numVerts);
	static void CalculateSphere(Sphere& sphereIn, Vert_Final_Skinned* verts, int numVerts);
};

#endif