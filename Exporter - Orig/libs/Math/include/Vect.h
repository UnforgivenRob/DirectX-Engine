#ifndef ENGINE_MATH_VECT_H
#define ENGINE_MATH_VECT_H

// Includes to handle SIMD register types
#include <xmmintrin.h>
#include <smmintrin.h> 
#include "Constants.h"
#include "Enum.h"
// forward declare
class Matrix;

class Vect
{
public:
	//Constructors
	Vect();
	Vect(float xVal, float yVal, float zVal);
	Vect(float xVal, float yVal, float zVal, float wVal);
	Vect(__m128 mIn);
	Vect(const Vect &t);

	//Destructor
	~Vect();

	//assignment operator
	void const operator = ( const Vect &tmp);

	//getter bracket operators
	const float& operator [](const x_enum val) const;
	const float& operator [](const y_enum val) const;
	const float& operator [](const z_enum val) const;
	const float& operator [](const w_enum val) const;

	//setter bracket operators
	float& operator [](const x_enum);
	float& operator [](const y_enum);
	float& operator [](const z_enum);
	float& operator [](const w_enum);

	//set functions
	void set(const float x, const float y, const float z);
	void set(const float x, const float y, const float z, const float w);
	void set(const Vect& in);

	//binary operators
	Vect operator + (const Vect &t) const;
	Vect operator - (const Vect &t) const;
	Vect operator * (const float f) const;
	void const operator += (const Vect &t);
	void const operator -= (const Vect &t);
	void const operator *= (const float f);

	//Unary operators
	Vect operator-();
	Vect operator+();

	//Comparison functions
	bool isEqual(const Vect& v2, const float tolerance = MATH_TOLERANCE);
	bool isZero(const float tolerance = MATH_TOLERANCE);

	//vector operations
	float dot(const Vect& v2) const;
	Vect cross(const Vect& v2) const;
	void norm();
	Vect getNorm() const;
	float mag() const;
	float magSqr() const;
	float getAngle(const Vect& v2);

	//vector * matrix operators
	const Vect operator * (const Matrix& m);
	void const operator *= (const Matrix& m);


private:

// Level 4 complains nameless struct/union ...
#pragma warning( disable : 4201)
	// anonymous union
	union 
	{
		__m128	m;

		// anonymous struct
		struct 
			{
			float x;
			float y;
			float z;
			float w;
			};
	};
};

//float * vect operator
Vect operator * (float f, const Vect& v);

#endif

