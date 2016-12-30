/** @file */ 
/*****************************************************************************/
/*                                                                           */
/* file: Quat.h                                                              */
/*                                                                           */
/*****************************************************************************/

#ifndef ENGINE_MATH_QUAT_H
#define ENGINE_MATH_QUAT_H

/*****************************************************************************/
/* Includes:                                                                 */
/*****************************************************************************/
#include "Matrix.h"
#include "Enum.h"


	/*************************************************************************
	*
	* This class contains functions and data structure for a 4-dimensional 
	* Quaternions.  The quaternions are internally stored interms of the 
	* imaginary components (qx,qy,qz) and the real components (qw).
	*
	* In the Math library, quaternions can be thought of as "rotational"
	* matrices, everything that you can represent with rotational matrices
	* you can use with quaternions.  Such as quaternion concatenation,
	* inverses, transpose, normalize, multiplying with matrices, and with
	* vectors.
	*  
	*  
	**************************************************************************/

class Quat 
{
public:
	//default constructor
	Quat();

	//specialized constructors
	Quat(const Matrix& inMat);
	Quat(const Quat& inQuat);
	Quat(const Vect& vectComp, const float realComp);
	Quat(const float qx, const float qy, const float qz, const float qw);
	Quat(const RotAxisAngleType, const Vect& axis, const float amount);
	Quat(const Rot3AxisType, const float xRot, const float yRot, const float zRot);
	Quat(const RotOrientType type, const Vect& inV1, const Vect& inV2);
	Quat(const RotType rot, const float angle);
	Quat(const MatrixSpecialType type);

	//default constructor
	~Quat();

	//assignment operator
	void operator = (const Quat& inQuat);

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

	//unary operators
	Quat operator +();
	Quat operator -();

	//binary operators
	Quat operator + (const Quat& t) const;
	Quat operator - (const Quat& t) const;
	Quat operator * (const Quat& t) const;
	Quat operator /	(const Quat& t) const;
	Quat operator + (const float f) const;
	Quat operator - (const float f) const;
	Quat operator * (const float f) const;
	Quat operator /	(const float f) const;
	void const operator += (const Quat& t);
	void const operator -= (const Quat& t);
	void const operator *= (const Quat& f);
	void const operator /= (const Quat& t);
	void const operator += (const float f);
	void const operator -= (const float f);
	void const operator *= (const float f);
	void const operator /= (const float f);

	//set functions
	void set(const float x, const float y, const float z, const float w);
	void set(const RotType type, const float angle);
	void set(const Matrix& inMat);
	void set(const RotAxisAngleType, const Vect& axis, const float amount);
	void set(const Rot3AxisType, const float xRot, const float yRot, const float zRot);
	void set(const RotOrientType type, const Vect& inV1, const Vect& inV2);
	void set(const Vect& vectComp, const float realComp);
	void set(const MatrixSpecialType type);

	//Comparison functions
	bool isEqual(const Quat& v2, const float tolerance = MATH_TOLERANCE);
	bool isZero(const float tolerance = MATH_TOLERANCE);

	//boolean operations
	bool isConjugateEqual(const Quat& inQuat, const float tolerance = MATH_TOLERANCE);
	bool isEquivalent(const Quat& inQuat, const float tolerance = MATH_TOLERANCE);
	bool isIdentity(const float tolerance = MATH_TOLERANCE);
	bool isNegEqual(const Quat& v2, const float tolerance = MATH_TOLERANCE);
	bool isNormalized(const float tolerance = MATH_TOLERANCE);

	//quaternion operations
	float dot(const Quat& v2) const;
	Quat norm();
	Quat getNorm() const;
	void conj();
	Quat getConj() const;
	void inv();
	Quat getInv() const;
	void T();
	Quat getT() const;
	void setVect(const Vect& inVec);
	void getVect(Vect& vIn);
	void getAxis(Vect& vIn);
	float getAngle() const;
	float mag() const;
	float magSquared() const;
	float invMag() const;

	void Lqcvq(const Vect& vIn, Vect& vOut);
	void Lqvqc(const Vect& vIn, Vect& vOut);
	Quat multByElement(const Quat& inQuat);


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
			float qx;
			float qy;
			float qz;
			float qw;
			};
	};
};

Quat operator + (const float f, const Quat& v);
Quat operator - (const float f, const Quat& v);
Quat operator * (const float f, const Quat& v);
Quat operator / (const float f, const Quat& v);


#endif // #ifndef QUAT_H

/**** END of Quat.h ********************************************************/
