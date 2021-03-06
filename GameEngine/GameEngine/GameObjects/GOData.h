#ifndef GODATA_H
#define GODATA_H

#include <MathEngine.h>

struct GOData
{
	GOData()
		: Scale( IDENTITY ), Rotation( IDENTITY ), Translation( IDENTITY ), incScale( IDENTITY ), incTrans( IDENTITY ), rotX( 0.0f ), rotY( 0.0f ), rotZ( 0.0f )
	{
	}

	Matrix Scale;
	Matrix Rotation;
	Matrix Translation;

	Matrix incScale;
	Matrix incTrans;

	float rotX;
	float rotY;
	float rotZ;	

	//overload operator new and delete to ensure aligned 16
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		return _mm_free(p);
	}
};

#endif