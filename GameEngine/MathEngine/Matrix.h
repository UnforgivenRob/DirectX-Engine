#ifndef ENGINE_MATH_MATRIX_H
#define ENGINE_MATH_MATRIX_H

// SIMD includes
#include <xmmintrin.h>
#include <smmintrin.h> 


#include <assert.h>
#include "Quat.h"
#include "Enum.h"

class Matrix
{
public:
	//constructors
	Matrix();
	Matrix(const Vect& inV0, const Vect& inV1, const Vect& inV2, const Vect& inV3);

	Matrix(const float inM0, const float inM1, const float inM2, const float inM3,
			const float inM4, const float inM5, const float inM6, const float inM7,
			const float inM8, const float inM9, const float inM10, const float inM11,
			const float inM12, const float inM13, const float inM14, const float inM15);

	Matrix(RotType rot, float angle);
	Matrix(MatrixScaleType, float xScale, float yScale, float zScale);
	Matrix(MatrixScaleType, const Vect& vIn);
	Matrix(MatrixTransType, const float xVal, const float yVal, const float zVal);
	Matrix(MatrixTransType, const Vect& vIn);
	Matrix(MatrixSpecialType type);
	Matrix(RotOrientType rotOrType, const Vect& inV1, const Vect& inV2);
	Matrix(Rot3AxisType, const float xRot, const float yRot, const float zRot);
	Matrix(RotAxisAngleType rotAxisType, const Vect& axis, const float amount);
	Matrix(const Quat& inQuat);
	Matrix(const Matrix& tmp);

	//destructor
	~Matrix();

	//assignment operator
	void const operator = (const Matrix& tmp);

	//getter functions
	Vect get(const MatrixRowType type);

	//setter functions
	void set(const MatrixSpecialType type);
	void set(const MatrixTransType, const float xval, const float yval, const float zval);
	void set(const MatrixTransType, const Vect& vIn);
	void set(const MatrixScaleType, const float xval, const float yval, const float zval);
	void set(const MatrixScaleType, const Vect& vIn);
	void set(const RotType type, const float val);
	void set(const MatrixRowType type, const Vect& vIn);
	void set(const Vect& v0In, const Vect& v1In, const Vect& v2In, const Vect& v3In);
	void set(const RotOrientType rotOrType, const Vect& inV1, const Vect& inV2);
	void set(const Rot3AxisType, const float xRot, const float yRot, const float zRot);
	void set(const RotAxisAngleType rotAxisType, const Vect& axis, const float amount);
	void set(const Quat& inQuat);

	//getter bracket operators 
	const float& operator [](const m0_enum val) const;
	const float& operator [](const m1_enum val) const;
	const float& operator [](const m2_enum val) const;
	const float& operator [](const m3_enum val) const;
	const float& operator [](const m4_enum val) const;
	const float& operator [](const m5_enum val) const;
	const float& operator [](const m6_enum val) const;
	const float& operator [](const m7_enum val) const;
	const float& operator [](const m8_enum val) const;
	const float& operator [](const m9_enum val) const;
	const float& operator [](const m10_enum val) const;
	const float& operator [](const m11_enum val) const;
	const float& operator [](const m12_enum val) const;
	const float& operator [](const m13_enum val) const;
	const float& operator [](const m14_enum val) const;
	const float& operator [](const m15_enum val) const;

	//setter bracket operators
	float& operator [](const m0_enum val);
	float& operator [](const m1_enum val);
	float& operator [](const m2_enum val);
	float& operator [](const m3_enum val);
	float& operator [](const m4_enum val);
	float& operator [](const m5_enum val);
	float& operator [](const m6_enum val);
	float& operator [](const m7_enum val);
	float& operator [](const m8_enum val);
	float& operator [](const m9_enum val);
	float& operator [](const m10_enum val);
	float& operator [](const m11_enum val);
	float& operator [](const m12_enum val);
	float& operator [](const m13_enum val);
	float& operator [](const m14_enum val);
	float& operator [](const m15_enum val);

	//Unary + - operators
	Matrix operator + ();
	Matrix operator - ();

	//binary addition, subtraction, multiplication operators
	Matrix const operator + (const Matrix &t) const;
	Matrix const operator - (const Matrix &t) const;
	Matrix const operator * (const Matrix& t) const;
	void const operator += (const Matrix &t);
	void const operator -= (const Matrix &t);
	void const operator *= (const Matrix& t);

	//float scaling operations
	Matrix const operator* (const float scale) const;
	void const operator *= (const float scale);

	//comparision functions
	bool isEqual(const Matrix& t, const float tolerance = MATH_TOLERANCE);
	bool isIdentity(const float tolerance = MATH_TOLERANCE);

	//determinant
	float det();
	
	//transpose
	void T();
	Matrix getT();

	//inverse
	void inv();
	Matrix getInv();

private:
	Matrix Adjugate();
// Level 4 complains nameless struct/union ...
#pragma warning( disable : 4201)

union 
		{
		struct 
			{
			Vect v0;
			Vect v1;
			Vect v2;
			Vect v3;
			};

		struct 
			{
			float m0;
			float m1;
			float m2;
			float m3;
			float m4;
			float m5;
			float m6;	
			float m7;
			float m8;
			float m9;
			float m10;
			float m11;
			float m12;
			float m13;
			float m14;
			float m15;
			};
		};
};

//float * matrix operator
Matrix const operator * (const float scale, const Matrix& v);

#endif