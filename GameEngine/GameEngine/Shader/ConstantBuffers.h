#ifndef CONSTANTBUFFERS_H
#define CONSTANTBUFFERS_H
#include "MathEngine.h"

struct BaseBuffer
{
	Matrix model;
	Matrix view;
	Matrix proj;
};

struct solidBuffer
{
	Matrix model;
	Matrix view;
	Matrix proj;
	Vect   color;
};
#endif