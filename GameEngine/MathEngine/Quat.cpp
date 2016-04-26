/*****************************************************************************/
/*                                                                           */
/* File: Quat.cpp                                                            */
/*                                                                           */
/* Quaterion Class                                                           */
/*                                                                           */
/*****************************************************************************/

#include "MathEngine.h"
#include <math.h>
#include <assert.h>

//default constructor
Quat::Quat()
	: qx(0.0f), qy(0.0f), qz(0.0f), qw(1.0f)
{
}

//specialized constructor from Matrix
Quat::Quat(const Matrix& inMat)
{
	this->set(inMat);
}

//copy constructor
Quat::Quat(const Quat& inQuat)
	: m(inQuat.m)
{
}

//constructor from vector component and real component
Quat::Quat(const Vect& vectComp, const float realComp)
	: qx(vectComp[x]), qy(vectComp[y]), qz(vectComp[z]), qw(realComp)
{
}

//constructor using float components
Quat::Quat(const float qx, const float qy, const float qz, const float qw)
	: qx(qx), qy(qy), qz(qz), qw(qw)
{
}

//constructor around a specified axis and amount
Quat::Quat(const RotAxisAngleType, const Vect& axis, const float amount)
{
	this->set(RotAxisAngleType::ROT_AXIS_ANGLE, axis, amount);
}

Quat::Quat(const Rot3AxisType, const float xRot, const float yRot, const float zRot)
{
	this->set(Rot3AxisType::ROT_XYZ, xRot, yRot, zRot);
}

//constructor to orient something given 2 specified vectors
Quat::Quat(const RotOrientType type, const Vect& inV1, const Vect& inV2)
{
	this->set(type, inV1, inV2);
}

//construct rotation around one of the three axis
Quat::Quat(const RotType rot, const float angle)
{
	this->set(rot, angle);
}

//constructor for identity or zero
Quat::Quat(const MatrixSpecialType type)
{
	this->set(type);
}

//default constructors
Quat::~Quat()
{
}

//assignment operator
void Quat::operator = (const Quat& inQuat)
{
	this->qx = inQuat.qx;
	this->qy = inQuat.qy;
	this->qz = inQuat.qz;
	this->qw = inQuat.qw;
}

//getter bracket operators
const float& Quat::operator [](const x_enum) const
{
	return this->qx;
}

const float& Quat::operator [](const y_enum) const 
{
	return this->qy;
}

const float& Quat::operator [](const z_enum) const
{
	return this->qz;
}

const float& Quat::operator [](const w_enum) const
{
	return this->qw;
}

//setter bracket operators
float& Quat::operator [](const x_enum)
{
	return this->qx;
}

float& Quat::operator [](const y_enum)
{
	return this->qy;
}

float& Quat::operator [](const z_enum)
{
	return this->qz;
}

float& Quat::operator [](const w_enum)
{
	return this->qw;
}

//TODO::
//unary + operator
Quat Quat::operator +()
{
	return Quat(*this);
}

//unary - operator
Quat Quat::operator -()
{
	return Quat(-this->qx, -this->qy, -this->qz, -this->qw);
}

Quat Quat::operator + (const Quat &t) const
{
	return Quat(this->qx + t.qx, this->qy + t.qy, this->qz + t.qz, this->qw + t.qw);
}


Quat Quat::operator - (const Quat &t) const
{
	return Quat(this->qx - t.qx, this->qy - t.qy, this->qz - t.qz, this->qw - t.qw);
}

Quat Quat::operator * (const Quat& t) const
{
	Vect vCompThis = Vect(this->qx, this->qy, this->qz);
	Vect vCompT =  Vect(t.qx, t.qy, t.qz);
	Vect vComp = vCompT.cross(vCompThis) + t.qw * vCompThis + this->qw * vCompT;
	float realComp = (this->qw * t.qw) - vCompThis.dot(vCompT);
	return Quat(vComp, realComp);
}

Quat Quat::operator / (const Quat& t) const
{
	return Quat(this->qx / t.qx, this->qy / t.qy, this->qz / t.qz, this->qw / t.qw);
}

Quat Quat::operator + (const float f) const
{
	return Quat(this->qx + f, this->qy + f, this->qz + f, this->qw + f);
}

Quat Quat::operator - (const float f) const
{
	return Quat(this->qx - f, this->qy - f, this->qz - f, this->qw - f);
}

Quat Quat::operator * (const float f) const
{
	return Quat(this->qx * f, this->qy * f, this->qz * f, this->qw * f);
}

Quat Quat::operator / (const float f) const
{
	return Quat(this->qx / f, this->qy / f, this->qz / f, this->qw / f);
}

void const Quat::operator += (const Quat &t)
{
	this->qx += t.qx;
	this->qy += t.qy;
	this->qz += t.qz;
	this->qw += t.qw;
}

void const Quat::operator -= (const Quat &t)
{
	this->qx -= t.qx;
	this->qy -= t.qy;
	this->qz -= t.qz;
	this->qw -= t.qw;
}

void const Quat::operator *= (const Quat& t)
{
	Vect vCompThis = Vect(this->qx, this->qy, this->qz);
	Vect vCompT =  Vect(t.qx, t.qy, t.qz);
	Vect vComp = vCompT.cross(vCompThis) + t.qw * vCompThis + this->qw * vCompT;
	float realComp = (this->qw * t.qw) - vCompThis.dot(vCompT);
	this->set(vComp, realComp);
}

void const Quat::operator /= (const Quat& t)
{
	this->qx /= t.qx;
	this->qy /= t.qy;
	this->qz /= t.qz;
	this->qw /= t.qw;
}

void const Quat::operator += (const float f)
{
	this->qx += f;
	this->qy += f;
	this->qz += f;
	this->qw += f;
}

void const Quat::operator -= (const float f)
{
	this->qx -= f;
	this->qy -= f;
	this->qz -= f;
	this->qw -= f;
}

void const Quat::operator *= (const float f)
{
	this->qx *= f;
	this->qy *= f;
	this->qz *= f;
	this->qw *= f;
}

void const Quat::operator /= (const float f)
{
	this->qx /= f;
	this->qy /= f;
	this->qz /= f;
	this->qw /= f;
}

Quat operator + (const float f, const Quat& q)
{
	return Quat(f + q[x], f + q[y], f + q[z], f + q[w]);
}

Quat operator - (const float f, const Quat& q)
{
	return Quat(f - q[x], f - q[y], f - q[z], f - q[w]);
}

Quat operator * (const float f, const Quat& q)
{
	return Quat(f * q[x], f * q[y], f * q[z], f * q[w]);
}

Quat operator / (const float f, const Quat& q)
{
	return Quat(f / q[x], f / q[y], f / q[z], f / q[w]);
}

Quat Quat::multByElement(const Quat& inQuat)
{
	return Quat(this->qx * inQuat.qx, this->qy * inQuat.qy, this->qz * inQuat.qz, this->qw * inQuat.qw);
}

//basic set function float by float
void Quat::set(const float x, const float y, const float z, const float w)
{
	this->qx = x;
	this->qy = y;
	this->qz = z;
	this->qw = w;
}

//set function based on rotation type and angle
void Quat::set(const RotType type, const float angle)
{
	this->qw = cosf(angle / 2.0f);
	float sine = sinf(angle/2.0f);
	switch(type)
	{
	case RotType::ROT_X:
		this->qx = 1.0f * sine;
		this->qy = 0.0f;
		this->qz = 0.0f;
		break;
	case RotType::ROT_Y:
		this->qx = 0.0f;
		this->qy = 1.0f * sine;
		this->qz = 0.0f;
		break;
	case RotType::ROT_Z:
		this->qx = 0.0f;
		this->qy = 0.0f;
		this->qz = 1.0f * sine;
		break;
	}
}

//matrix set function
void Quat::set(const Matrix& inMat)
{
	float tr = inMat[m0] + inMat[m5] + inMat[m10];
	if(tr > 0)
	{
		float f = 2 * sqrtf(tr + 1.0f);
		this->qx = -(inMat[m9] - inMat[m6]) / f;
		this->qy = -(inMat[m2] - inMat[m8]) / f;
		this->qz = -(inMat[m4] - inMat[m1]) / f;
		this->qw = 0.25f * f;
	}
	else if ((inMat[m0] > inMat[m5]) && (inMat[m0] > inMat[m10]))
	{
		float f = 2 * sqrtf(1.0f + inMat[m0] - inMat[m5] - inMat[m10]);

		this->qx = 0.25f * f;
		this->qy = (inMat[m1] + inMat[m4]) / f;
		this->qz = (inMat[m2] + inMat[m8]) / f;
		this->qw = -(inMat[m9] - inMat[m6]) / f;
	}
	else if (inMat[m5] > inMat[m10])
	{
		float f = 2.0f * sqrtf(1.0f + inMat[m5] - inMat[m0] - inMat[m10]);
		
		this->qx = (inMat[m1] + inMat[m4]) / f;
		this->qy = 0.25f * f;
		this->qz = (inMat[m6] + inMat[m9]) / f;
		this->qw = -(inMat[m2] - inMat[m8]) / f;
	}
	else
	{
		float f = 2 * sqrtf(1.0f + inMat[m10] - inMat[m0] - inMat[m5]);
		
		this->qx = (inMat[m2] + inMat[m8]) / f;
		this->qy = (inMat[m6] + inMat[m9]) / f;
		this->qz = 0.25f * f;
		this->qw = -(inMat[m4] - inMat[m1]) / f;
	}
}

//sets quaternion for rotation around specified axis of a given amount
void Quat::set(const RotAxisAngleType, const Vect& axis, const float angle)
{
	Vect vTemp = axis * sinf(angle / 2.0f);
	vTemp.norm();

	this->qx = vTemp[x]/2.0f;
	this->qy = vTemp[y]/2.0f;
	this->qz = vTemp[z]/2.0f;
	this->qw = cosf(angle / 2.0f);
}

//constructs quaternion for rotation specified by amount around each axis
void Quat::set(const Rot3AxisType, const float xRot, const float yRot, const float zRot)
{
	*this = Quat(ROT_X, xRot) * Quat(ROT_Y, yRot) * Quat(ROT_Z, zRot);
}

//sets quaternion to orient something given 2 specified basis vectors
void Quat::set(const RotOrientType type, const Vect& dof, const Vect& up)
{
	*this = Quat(Matrix(type, dof, up));
}

//sets quaternion given a specified vector and real component
void Quat::set(const Vect& vectComp, const float realComp)
{
	this->qx = vectComp[x];
	this->qy = vectComp[y];
	this->qz = vectComp[z];
	this->qw = realComp;
}

void Quat::set(const MatrixSpecialType type)
{
	switch(type)
	{
	case MatrixSpecialType::IDENTITY:
		this->qx = 0.0f;
		this->qy = 0.0f;
		this->qz = 0.0f;
		this->qw = 1.0f;
		break;
	case MatrixSpecialType::ZERO:
		this->qx = 0.0f;
		this->qy = 0.0f;
		this->qz = 0.0f;
		this->qw = 0.0f;
		break;
	}
}

//quaternion dot product
float Quat::dot(const Quat& q2) const
{
	return this->qx * q2.qx + this->qy * q2.qy + this->qz * q2.qz + this->qw * q2.qw;
}

//normalizes quaternion
Quat Quat::norm()
{
	float f = this->invMag();
	this->qx *= f;
	this->qy *= f;
	this->qz *= f;
	this->qw *= f;
	return Quat(*this);
}

//returns the norm of this quaternion
Quat Quat::getNorm() const
{
	float f = this->invMag();

	return Quat(this->qx * f, this->qy * f, this->qz * f, this->qw * f);
}

//convert this to the conjugate
void Quat::conj()
{
	this->qx *= -1.0f;
	this->qy *= -1.0f;
	this->qz *= -1.0f;
}

//get the conjugate of the quaternion
Quat Quat::getConj() const 
{
	return Quat(-this->qx, - this->qy, -this->qz, this->qw);
}

void Quat::inv()
{
	float invmag = 1.0f /this->magSquared();
	this->qx *= -invmag;
	this->qy *= -invmag;
	this->qz *= -invmag;
	this->qw *= invmag;
}

Quat Quat::getInv() const
{
	float invmag = 1.0f /this->magSquared();
	return Quat(-this->qx * invmag, -this->qy * invmag, -this->qz * invmag, this->qw * invmag);
}

void Quat::T()
{
	this->qx *= -1.0f;
	this->qy *= -1.0f;
	this->qz *= -1.0f;
}

Quat Quat::getT() const
{
	return Quat(-this->qx, - this->qy, -this->qz, this->qw);
}


void Quat::setVect(const Vect& inVec)
{
	this->qx = inVec[x];
	this->qy = inVec[y];
	this->qz = inVec[z];
}

void Quat::getVect(Vect& vIn)
{
	vIn[x] = this->qx;
	vIn[y] = this->qy;
	vIn[z] = this->qz;
	vIn[w] = 1.0f;
}

void Quat::getAxis(Vect& vIn)
{
	vIn[x] = this->qx;
	vIn[y] = this->qy;
	vIn[z] = this->qz;
	vIn[w] = 1.0f;
}

float Quat::getAngle() const
{
	return acosf(this->qw) * 2.0f;
}

float Quat::mag() const
{
	return sqrtf(this->magSquared());
}

float Quat::magSquared() const
{
	return (this->qx * this->qx) + (this->qy * this->qy) + (this->qz * this->qz) + (this->qw * this->qw);
}

float Quat::invMag() const
{
	return 1.0f/this->mag();
}

//tests if this and q2 are aqual (within a preset tolerance)
bool Quat::isEqual(const Quat& q2, const float tolerance)
{
	return (Util::isEqual(this->qx, q2.qx, tolerance))
			 &&(Util::isEqual(this->qy, q2.qy, tolerance))
			 &&(Util::isEqual(this->qz, q2.qz, tolerance))
			 &&(Util::isEqual(this->qw, q2.qw, tolerance));
}

//tests if this is zero (within a preset tolerance)
bool Quat::isZero(const float tolerance)
{
		return (Util::isZero(this->qx,  tolerance))
			 &&(Util::isZero(this->qy,  tolerance))
			 &&(Util::isZero(this->qz,  tolerance))
			 &&(Util::isZero(this->qw, tolerance));
}

bool Quat::isConjugateEqual(const Quat& inQuat, const float tolerance)
{
	Quat inQuatConj = inQuat.getConj();
	return (Util::isEqual( this->qx, inQuatConj.qx, tolerance))
			 &&(Util::isEqual( this->qy, inQuatConj.qy, tolerance))
			 &&(Util::isEqual( this->qz, inQuatConj.qz, tolerance))
			 &&(Util::isEqual( this->qw, inQuatConj.qw, tolerance));
}

bool Quat::isEquivalent(const Quat& inQuat, const float tolerance)
{
	if( inQuat.qx * this->qx > 0.0f )
	{
		return (Util::isEqual(this->qx, inQuat.qx, tolerance))
			 &&(Util::isEqual(this->qy, inQuat.qy, tolerance))
			 &&(Util::isEqual(this->qz, inQuat.qz, tolerance))
			 &&(Util::isEqual(this->qw, inQuat.qw, tolerance));
	}
	else
	{
		//one or the other is negative
		return (Util::isEqual(fabs(this->qx), fabs(inQuat.qx), tolerance))
			 &&(Util::isEqual(fabs(this->qy), fabs(inQuat.qy), tolerance))
			 &&(Util::isEqual(fabs(this->qz), fabs(inQuat.qz), tolerance))
			 &&(Util::isEqual(fabs(this->qw), fabs(inQuat.qw), tolerance));
	}
}

bool Quat::isIdentity(const float tolerance)
{
	return (Util::isZero(this->qx,  tolerance))
		 &&(Util::isZero(this->qy,  tolerance))
		 &&(Util::isZero(this->qz,  tolerance))
		 &&(Util::isOne(this->qw, tolerance));
}

bool Quat::isNegEqual(const Quat& q2, const float tolerance)
{
	return (Util::isEqual(this->qx, -q2.qx, tolerance))
			 &&(Util::isEqual(this->qy, -q2.qy, tolerance))
			 &&(Util::isEqual(this->qz, -q2.qz, tolerance))
			 &&(Util::isEqual(this->qw, -q2.qw, tolerance));
}

bool Quat::isNormalized(const float tolerance)
{
	return Util::isOne(this->mag(), tolerance);
}

void Quat::Lqcvq(const Vect& vIn, Vect& vOut)
{
	Vect vectComp;
	this->getVect(vectComp);

	vOut = (2.0f * qw) * (vectComp.cross(vIn)) + (qw * qw - vectComp.dot(vectComp)) * vIn + (2.0f * vectComp.dot(vIn)) * vectComp;
}

void Quat::Lqvqc(const Vect& vIn, Vect& vOut)
{
	Vect vectComp;
	this->getVect(vectComp);

	vOut = (2.0f * qw) * (vIn.cross(vectComp)) + (qw * qw - vectComp.dot(vectComp)) * vIn + (2.0f * vectComp.dot(vIn)) * vectComp;
}