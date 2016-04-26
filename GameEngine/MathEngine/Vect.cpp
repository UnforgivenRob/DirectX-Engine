#include "MathEngine.h"
#include <math.h>
#include <assert.h>


// do your magic
Vect::Vect()
	: x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{
}

//XYZ Specialized constructor
Vect::Vect(float xVal, float yVal, float zVal)
	: x(xVal), y(yVal), z(zVal), w(1.0f)
{
}

//4-value Specialized constructor
Vect::Vect(float xVal, float yVal, float zVal, float wVal)
	: x(xVal), y(yVal), z(zVal), w(wVal)
{
}

//SIMD _m128 constructor
Vect::Vect(__m128 mIn)
	: m(mIn), w(1.0f)
{
}

//copy constructor
Vect::Vect(const Vect &t)
	: x(t.x), y(t.y), z(t.z), w(t.w)
{
}

//Destructor
Vect::~Vect()
{
}

void const Vect::operator = ( const Vect &tmp)
{
	if (this != &tmp)
	{
		this->x = tmp.x;
		this->y = tmp.y;
		this->z = tmp.z;
		this->w = tmp.w;
	}
}

//getter bracket operators
const float& Vect::operator [](const x_enum) const
{
	return this->x;
}

const float& Vect::operator [](const y_enum) const 
{
	return this->y;
}

const float& Vect::operator [](const z_enum) const
{
	return this->z;
}

const float& Vect::operator [](const w_enum) const
{
	return this->w;
}

//setter bracket operators
float& Vect::operator [](const x_enum)
{
	return this->x;
}

float& Vect::operator [](const y_enum)
{
	return this->y;
}

float& Vect::operator [](const z_enum)
{
	return this->z;
}

float& Vect::operator [](const w_enum)
{
	return this->w;
}

//set functions
//xyz setter
void Vect::set(const float x, const float y, const float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = 1.0f;
}

//4val setter
void Vect::set(const float x, const float y, const float z, const float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

//Vect setter
void Vect::set(const Vect& in)
{
	this->x = in.x;
	this->y = in.y;
	this->z = in.z;
	this->w = in.w;
}

//binary + and - operators
Vect Vect::operator + (const Vect &t) const
{
	return Vect(this->x + t.x, this->y + t.y, this->z + t.z);
}

void const Vect::operator += (const Vect &t)
{
	this->x += t.x;
	this->y += t.y;
	this->z += t.z;
	this->w = 1.0f;
}

Vect Vect::operator - (const Vect &t) const
{
	return Vect(this->x - t.x, this->y - t.y, this->z - t.z);
}

void const Vect::operator -= (const Vect &t)
{
	this->x -= t.x;
	this->y -= t.y;
	this->z -= t.z;
	this->w = 1.0f;
}

Vect Vect::operator * (const float f) const
{
	return Vect(this->x * f, this->y * f, this->z * f);
}

void const Vect::operator *= (const float f)
{
	this->x *= f;
	this->y *= f;
	this->z *= f;
	this->w = 1.0f;
}

Vect Vect::operator-()
{
	return Vect(-this->x, -this->y, -this->z);
}

Vect Vect::operator+()
{
	return Vect(+this->x, +this->y, +this->z);
}

//tests if this and v2 are aqual (within a preset tolerance)
bool Vect::isEqual(const Vect& v2, const float tolerance)
{
	return (Util::isEqual(this->x, v2.x, tolerance))
			 &&(Util::isEqual(this->y, v2.y, tolerance))
			 &&(Util::isEqual(this->z, v2.z, tolerance))
			 &&(Util::isEqual(this->w, v2.w, tolerance));
}

//tests if this is zero (within a preset tolerance)
bool Vect::isZero(const float tolerance)
{
		return (Util::isZero(this->x,  tolerance))
			 &&(Util::isZero(this->y,  tolerance))
			 &&(Util::isZero(this->z,  tolerance))
			 &&(Util::isOne(this->w, tolerance));
}

float Vect::dot(const Vect& v2) const
{
	return (this->x * v2.x + this->y * v2.y + this->z * v2.z);
}

Vect Vect::cross(const Vect& v2) const
{
	return Vect(this->y * v2.z - this->z * v2.y,
				this->z * v2.x - this->x * v2.z,
				this->x * v2.y - this->y * v2.x);
}

void Vect::norm()
{
	//get magnitude squared
	float magSqrd = this->x * this->x + this->y * this->y + this->z * this->z;	
	//make sure there is no divide by zero
	assert(Util::isNonZero(magSqrd, ENGINE_MATH_TOLERANCE));
	
	//multiply this by the inverse squareroot / divide by square root
	float invSqrt = 1/sqrtf(magSqrd);

	this->x *= invSqrt;
	this->y *= invSqrt;
	this->z *= invSqrt;
}

Vect Vect::getNorm() const
{
	//calc mag squared 
	float magSqrd = this->x * this->x + this->y * this->y + this->z * this->z;
	if(!Util::isNonZero(magSqrd, ENGINE_MATH_TOLERANCE))
	{
		return Vect(0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	//divide this by the square root of magsqrd
	float invSqrt = 1/sqrtf(magSqrd);

	return Vect(this->x * invSqrt, this->y * invSqrt, this->z * invSqrt);
}

float Vect::mag() const
{
	return sqrtf( this->x * this->x + this->y * this->y + this->z * this->z);
}

float Vect::magSqr() const
{
	return this->x * this->x + this->y * this->y + this->z * this->z;
}

float Vect::getAngle(const Vect& v2)
{
	float dot = this->dot(v2);
	return acosf(dot / (this->mag() * v2.mag()));
}

const Vect Vect::operator * (const Matrix& m)
{
	return Vect(this->x * m[m0] + this->y * m[m4] + this->z * m[m8] + this->w * m[m12],
				this->x * m[m1] + this->y * m[m5] + this->z * m[m9] + this->w * m[m13],
				this->x * m[m2] + this->y * m[m6] + this->z * m[m10] + this->w * m[m14],
				this->x * m[m3] + this->y * m[m7] + this->z * m[m11] + this->w * m[m15]);
}

void const Vect::operator *= (const Matrix& m)
{
	float tmpX = this->x * m[m0] + this->y * m[m4] + this->z * m[m8] + this->w * m[m12];
	float tmpY = this->x * m[m1] + this->y * m[m5] + this->z * m[m9] + this->w * m[m13];
	float tmpZ = this->x * m[m2] + this->y * m[m6] + this->z * m[m10] + this->w * m[m14];
	float tmpW = this->x * m[m3] + this->y * m[m7] + this->z * m[m11] + this->w * m[m15];

	this->x = tmpX;
	this->y = tmpY;
	this->z = tmpZ;
	this->w = tmpW;
}

Vect operator * (float f, const Vect& v)
{
	return Vect(v[x] * f, v[y] * f, v[z] * f);
}