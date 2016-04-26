#include "MathEngine.h"
#include <math.h>

//constructor
Matrix::Matrix()
	: m0(0.0f), m1(0.0f), m2(0.0f), m3(0.0f), 
	  m4(0.0f), m5(0.0f), m6(0.0f), m7(0.0f), 
	  m8(0.0f), m9(0.0f), m10(0.0f), m11(0.0f),
	  m12(0.0f), m13(0.0f), m14(0.0f), m15(0.0f)
{
}

//specialized constructor
Matrix::Matrix(const Vect& inV0, const Vect& inV1, const Vect& inV2, const Vect& inV3)
	: v0(inV0), v1(inV1), v2(inV2), v3(inV3)
{
}

Matrix::Matrix(const float inM0, const float inM1, const float inM2, const float inM3,
				const float inM4, const float inM5, const float inM6, const float inM7,
				const float inM8, const float inM9, const float inM10, const float inM11,
				const float inM12, const float inM13, const float inM14, const float inM15)
	: m0(inM0),   m1(inM1),   m2(inM2),   m3(inM3), 
	  m4(inM4),   m5(inM5),   m6(inM6),   m7(inM7), 
	  m8(inM8),	  m9(inM9),   m10(inM10), m11(inM11),
	  m12(inM12), m13(inM13), m14(inM14), m15(inM15)
{
}

//Rotation Constructors
Matrix::Matrix(RotType rot, float angle)
	: m3(0.0f), m7(0.0f), m11(0.0f), m12(0.0f), m13(0.0f), m14(0.0f), m15(1.0f)
{
	float cosine = cosf(angle);
	float sine = sinf(angle);

	switch (rot)
	{
	case(ROT_X):
		this->m0 = 1.0f;
		this->m1 = 0.0f;
		this->m2 = 0.0f;
		this->m4 = 0.0f;
		this->m5 = cosine;
		this->m6 = sine;
		this->m8 = 0.0f;
		this->m9 = -sine;
		this->m10 = cosine;
		break;
	case(ROT_Y):
		this->m0 = cosine;
		this->m1 = 0.0f;
		this->m2 = -sine;
		this->m4 = 0.0f;
		this->m5 = 1.0f;
		this->m6 = 0.0f;
		this->m8 = sine;
		this->m9 = 0.0f;
		this->m10 = cosine;
		break;
	case(ROT_Z):
		this->m0 = cosine;
		this->m1 = sine;
		this->m2 = 0.0f;
		this->m4 = -sine;
		this->m5 = cosine;
		this->m6 = 0.0f;
		this->m8 = 0.0f;
		this->m9 = 0.0f;
		this->m10 = 1.0f;
		break;
	default:
		assert(0);
		break;
	}
}

//scale matrix constructor
Matrix::Matrix(MatrixScaleType, float xScale, float yScale, float zScale)
	: m0(xScale), m1(0.0f), m2(0.0f), m3(0.0f), 
	  m4(0.0f), m5(yScale), m6(0.0f), m7(0.0f),
	  m8(0.0f), m9(0.0f),m10(zScale), m11(0.0f),
	  m12(0.0f), m13(0.0f), m14(0.0f), m15(1.0f)
{
}

//scale matrix constructor
Matrix::Matrix(MatrixScaleType, const Vect& vIn)
	: m0(vIn[x]), m1(0.0f), m2(0.0f), m3(0.0f), 
	  m4(0.0f), m5(vIn[y]), m6(0.0f), m7(0.0f),
	  m8(0.0f), m9(0.0f), m10(vIn[z]), m11(0.0f),
	  m12(0.0f), m13(0.0f), m14(0.0f), m15(1.0f)
{
}

//trans matrix constructor
Matrix::Matrix(MatrixTransType, const float xVal, const float yVal, const float zVal)
	: m0(1.0f), m1(0.0f), m2(0.0f), m3(0.0f), 
	  m4(0.0f), m5(1.0f), m6(0.0f), m7(0.0f),
	  m8(0.0f), m9(0.0f),m10(1.0f), m11(0.0f),
	  m12(xVal), m13(yVal), m14(zVal), m15(1.0f)
{
}

//trans matrix constructor
Matrix::Matrix(MatrixTransType, const Vect& vIn)
	: m0(1.0f), m1(0.0f), m2(0.0f), m3(0.0f), 
	  m4(0.0f), m5(1.0f), m6(0.0f), m7(0.0f),
	  m8(0.0f), m9(0.0f),m10(1.0f), m11(0.0f),
	  v3(vIn)
{
}

//Identity and Zero constructors
Matrix::Matrix(MatrixSpecialType type)
	: m1(0.0f), m2(0.0f), m3(0.0f), 
		m4(0.0f), m6(0.0f), m7(0.0f),
		m8(0.0f), m9(0.0f), m11(0.0f),
		m12(0.0f), m13(0.0f), m14(0.0f)
{
	switch (type)
	{
	case (IDENTITY):
		m0 = 1.0f;
		m5 = 1.0f;
		m10 = 1.0f;
		m15 = 1.0f;
		break;
	case (ZERO):
		m0 = 0.0f;
		m5 = 0.0f;
		m10 = 0.0f;
		m15 = 0.0f;
		break;
	default:
		assert(0);
		break;
	}
}

Matrix::Matrix(RotOrientType rotOrType, const Vect& inV1, const Vect& inV2)
{
	this->set(rotOrType, inV1, inV2);
}

//Constructor for rotation matrix around arbitrary axis
Matrix::Matrix(RotAxisAngleType, const Vect& axis, const float amount)
{
	this->set(RotAxisAngleType::ROT_AXIS_ANGLE, axis, amount);
}

//Constructor for rotation matrix based on 3 axis
Matrix::Matrix(Rot3AxisType, const float xRot, const float yRot, const float zRot)
{
	this->set(Rot3AxisType::ROT_XYZ, xRot, yRot, zRot);
}

//constructor from quaternion
Matrix::Matrix(const Quat& inQuat)
{
	this->set(inQuat);
}

//copy constructor
Matrix::Matrix(const Matrix& tmp)
	: v0(tmp.v0), v1(tmp.v1), v2(tmp.v2), v3(tmp.v3) 
{
}

//destructor
Matrix::~Matrix()
{
}

//assignment operator
void const Matrix::operator = (const Matrix& tmp)
{
	this->v0 = tmp.v0;
	this->v1 = tmp.v1;
	this->v2 = tmp.v2;
	this->v3 = tmp.v3;
}

//getter functions
Vect Matrix::get(const MatrixRowType type)
{
	switch (type)
	{
	case (ROW_0):
		return this->v0;
	case (ROW_1):
		return this->v1;
	case (ROW_2):
		return this->v2;
	case (ROW_3):
		return this->v3;
	default:
		assert(0);
		return Vect(); //Should never be reached
	}
}
	
//setter functions
void Matrix::set(const MatrixSpecialType type)
{
	*this = Matrix(type);
}

//trans Matrix setters
void Matrix::set(const MatrixTransType, const float xval, const float yval, const float zval)
{
	*this = Matrix(TRANS, xval, yval, zval);
}

void Matrix::set(const MatrixTransType, const Vect& vIn)
{
	*this = Matrix(TRANS, vIn);
}

//scale matrix setters
void Matrix::set(const MatrixScaleType, const float xval, const float yval, const float zval)
{
	*this = Matrix(SCALE, xval, yval, zval);
}

void Matrix::set(const MatrixScaleType, const Vect& vIn)
{
	*this = Matrix(SCALE, vIn);
}

//rot matrix setter
void Matrix::set(const RotType type, const float val)
{
	*this = Matrix(type, val);
}

//row setting function
void Matrix::set(const MatrixRowType type, const Vect& vIn)
{
	switch (type) 
	{
	case(ROW_0):
		this->v0 = vIn;
		break;
	case(ROW_1):
		this->v1 = vIn;
		break;
	case(ROW_2):
		this->v2 = vIn;
		break;
	case(ROW_3):
		this->v3 = vIn;
		break;
	default:
		assert(0);
		break;
	}
}

void Matrix::set(const Vect& v0In, const Vect& v1In, const Vect& v2In, const Vect& v3In)
{
	this->v0 = v0In;
	this->v1 = v1In;
	this->v2 = v2In;
	this->v3 = v3In;
}

void Matrix::set(const RotOrientType rotOrType, const Vect& dof, const Vect& up)
{
	const Vect Dof = dof.getNorm(); 
	Vect right = dof.cross(up).getNorm();
	Vect Up = right.cross(Dof).getNorm();
	switch(rotOrType)
	{
	case RotOrientType::ROT_ORIENT:
		this->v0 = -right;
		this->v1 = Up;
		this->v2 = Dof;
		this->v3 = Vect(0.0f, 0.0f, 0.0f, 1.0f);
		this->m3 = 0.0f;
		this->m7 = 0.0f;
		this->m11 = 0.0f;
		break;
	case RotOrientType::ROT_INVERSE_ORIENT:
		this->m0 = -right[x];
		this->m1 = Up[x];
		this->m2 = Dof[x];
		this->m3 = 0.0f;
		this->m4 = -right[y];
		this->m5 = Up[y];
		this->m6 = Dof[y];
		this->m7  = 0.0f;
		this->m8 = -right[z];
		this->m9 = Up[z];
		this->m10 = Dof[z];
		this->m11 = 0.0f;
		this->v3 = Vect(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	}	
}

void Matrix::set(Rot3AxisType, const float xRot, const float yRot, const float zRot)
{
	*this = Matrix(ROT_X, xRot) * Matrix(ROT_Y, yRot) * Matrix(ROT_Z, zRot);
}

void Matrix::set(RotAxisAngleType, const Vect& axis, const float angle)
{
	float invXYZ = 1.0f / axis.dot(axis);
	float sqrRtXYZ = sqrtf(axis.dot(axis));
	float xsqr = axis[x] * axis[x];
	float ysqr = axis[y] * axis[y];
	float zsqr = axis[z] * axis[z];

	this->m0 = (xsqr + ((ysqr + zsqr) * cosf(angle))) * invXYZ;
	this->m1 = (((axis[x] * axis[y]) * (1.0f - cosf(angle))) + (axis[z] * sqrRtXYZ * sinf(angle))) * invXYZ;
	this->m2 = (((axis[x] * axis[z]) * (1.0f - cosf(angle))) - (axis[y] * sqrRtXYZ * sinf(angle))) * invXYZ;
	this->m3 = 0.0f;
	this->m4 = (((axis[x] * axis[y]) * (1.0f - cosf(angle))) - (axis[z] * sqrRtXYZ * sinf(angle))) * invXYZ;
	this->m5 = (ysqr + ((xsqr + zsqr) * cosf(angle))) * invXYZ;
	this->m6 = (((axis[y] * axis[z]) * (1.0f - cosf(angle))) + (axis[x] * sqrRtXYZ * sinf(angle))) * invXYZ;
	this->m7 = 0.0f;
	this->m8 = (((axis[x] * axis[z]) * (1.0f - cosf(angle))) + (axis[y] * sqrRtXYZ * sinf(angle))) * invXYZ;
	this->m9 = (((axis[y] * axis[z]) * (1.0f - cosf(angle))) - (axis[x] * sqrRtXYZ * sinf(angle))) * invXYZ;
	this->m10 = (zsqr + ((xsqr + ysqr) * cosf(angle))) * invXYZ;
	this->m11 = 0.0f;
	this->m12 = 0.0f;
	this->m13 = 0.0f;
	this->m14 = 0.0f;
	this->m15 = 1.0f;
}

void Matrix::set(const Quat& inQuat)
{
	this->m0 = 1.0f - 2.0f * (inQuat[y] * inQuat[y] + inQuat[z] * inQuat[z]);
	this->m1 = 2.0f * (inQuat[x] * inQuat[y] + inQuat[w] * inQuat[z]);
	this->m2 = 2.0f * (inQuat[x] * inQuat[z] - inQuat[w] * inQuat[y]);
	this->m3 = 0.0f;
	this->m4 = 2.0f * (inQuat[x] * inQuat[y] - inQuat[w] * inQuat[z]);
	this->m5 = 1.0f - 2.0f * (inQuat[x] * inQuat[x] + inQuat[z] * inQuat[z]);
	this->m6 = 2.0f * (inQuat[y] * inQuat[z] + inQuat[w] * inQuat[x]);
	this->m7 = 0.0f;
	this->m8 = 2.0f * (inQuat[x] * inQuat[z] + inQuat[w] * inQuat[y]);
	this->m9 = 2.0f * (inQuat[y] * inQuat[z] - inQuat[w] * inQuat[x]);
	this->m10 = 1.0f - 2.0f * (inQuat[x] * inQuat[x] + inQuat[y] * inQuat[y]);
	this->m11 = 0.0f;
	this->m12 = 0.0f;
	this->m13 = 0.0f;
	this->m14 = 0.0f;
	this->m15 = 1.0f;
}

const float& Matrix::operator [](const m0_enum) const
{
	return this->m0;
}

const float& Matrix::operator [](const m1_enum) const
{
	return this->m1;
}

const float& Matrix::operator [](const m2_enum) const
{
	return this->m2;
}

const float& Matrix::operator [](const m3_enum) const
{
	return this->m3;
}

const float& Matrix::operator [](const m4_enum) const
{
	return this->m4;
}

const float& Matrix::operator [](const m5_enum) const
{
	return this->m5;
}

const float& Matrix::operator [](const m6_enum) const
{
	return this->m6;
}
const float& Matrix::operator [](const m7_enum) const
{
	return this->m7;
}

const float& Matrix::operator [](const m8_enum) const
{
	return this->m8;
}

const float& Matrix::operator [](const m9_enum) const
{
	return this->m9;
}

const float& Matrix::operator [](const m10_enum) const
{
	return this->m10;
}

const float& Matrix::operator [](const m11_enum) const
{
	return this->m11;
}

const float& Matrix::operator [](const m12_enum) const
{
	return this->m12;
}

const float& Matrix::operator [](const m13_enum) const
{
	return this->m13;
}

const float& Matrix::operator [](const m14_enum) const
{
	return this->m14;
}

const float& Matrix::operator [](const m15_enum) const
{
	return this->m15;
}

//setter bracket operators================================================================
float& Matrix::operator [](const m0_enum) 
{
	return this->m0;
}

float& Matrix::operator [](const m1_enum) 
{
	return this->m1;
}

float& Matrix::operator [](const m2_enum) 
{
	return this->m2;
}

float& Matrix::operator [](const m3_enum) 
{
	return this->m3;
}

float& Matrix::operator [](const m4_enum) 
{
	return this->m4;
}

float& Matrix::operator [](const m5_enum) 
{
	return this->m5;
}

float& Matrix::operator [](const m6_enum) 
{
	return this->m6;
}
	
float& Matrix::operator [](const m7_enum) 
{
	return this->m7;
}

float& Matrix::operator [](const m8_enum) 
{
	return this->m8;
}

float& Matrix::operator [](const m9_enum) 
{
	return this->m9;
}

float& Matrix::operator [](const m10_enum)
{
	return this->m10;
}

float& Matrix::operator [](const m11_enum)
{
	return this->m11;
}

float& Matrix::operator [](const m12_enum)
{
	return this->m12;
}

float& Matrix::operator [](const m13_enum)
{
	return this->m13;
}

float& Matrix::operator [](const m14_enum)
{
	return this->m14;
}

float& Matrix::operator [](const m15_enum)
{
	return this->m15;
}

//Unary + - operators========================================================================================
Matrix Matrix::operator + ()
{
	return Matrix(+this->m0, +this->m1, +this->m2, +this->m3,
					+this->m4, +this->m5, +this->m6, +this->m7,
					+this->m8, +this->m9, +this->m10, +this->m11,
					+this->m12, +this->m13, +this->m14, +this->m15);
}

Matrix Matrix::operator - ()
{
	return Matrix(-this->m0, -this->m1, -this->m2, -this->m3,
					-this->m4, -this->m5, -this->m6, -this->m7,
					-this->m8, -this->m9, -this->m10, -this->m11,
					-this->m12, -this->m13, -this->m14, -this->m15);
}

//binary addition and subtraction operators
Matrix const Matrix::operator + (const Matrix &t) const
{
	return Matrix(this->m0 + t.m0, this->m1 + t.m1, this->m2 + t.m2, this->m3 + t.m3,
					this->m4 + t.m4, this->m5 + t.m5, this->m6 + t.m6, this->m7 + t.m7,
					this->m8 + t.m8, this->m9 + t.m9, this->m10 + t.m10, this->m11 + t.m11,
					this->m12 + t.m12, this->m13 + t.m13, this->m14 + t.m14, this->m15 + t.m15);
}

Matrix const Matrix::operator - (const Matrix &t) const
{
	return Matrix(this->m0 - t.m0, this->m1 - t.m1, this->m2 - t.m2, this->m3 - t.m3,
					this->m4 - t.m4, this->m5 - t.m5, this->m6 - t.m6, this->m7 - t.m7,
					this->m8 - t.m8, this->m9 - t.m9, this->m10 - t.m10, this->m11 - t.m11,
					this->m12 - t.m12, this->m13 - t.m13, this->m14 - t.m14, this->m15 - t.m15);
}

void const Matrix::operator += (const Matrix &t)
{
	this->m0 += t.m0;	this->m1 += t.m1;	this->m2 += t.m2;	this->m3 += t.m3;
	this->m4 += t.m4;	this->m5 += t.m5;	this->m6 += t.m6;	this->m7 += t.m7;
	this->m8 += t.m8;	this->m9 += t.m9;	this->m10 += t.m10;	this->m11 += t.m11;
	this->m12 += t.m12;	this->m13 += t.m13;	this->m14 += t.m14;	this->m15 += t.m15;
}

void const Matrix::operator -= (const Matrix &t)
{
	this->m0 -= t.m0;	this->m1 -= t.m1;	this->m2 -= t.m2;	this->m3 -= t.m3;
	this->m4 -= t.m4;	this->m5 -= t.m5;	this->m6 -= t.m6;	this->m7 -= t.m7;
	this->m8 -= t.m8;	this->m9 -= t.m9;	this->m10 -= t.m10;	this->m11 -= t.m11;
	this->m12 -= t.m12;	this->m13 -= t.m13;	this->m14 -= t.m14;	this->m15 -= t.m15;
}

//float scaling operations
Matrix const Matrix::operator* (const float scale) const
{
	return Matrix(this->m0 * scale, this->m1 * scale, this->m2 * scale, this->m3 * scale,
					this->m4 * scale, this->m5 * scale, this->m6 * scale, this->m7 * scale,
					this->m8 * scale, this->m9 * scale, this->m10 * scale, this->m11 * scale,
					this->m12 * scale, this->m13 * scale, this->m14 * scale, this->m15 * scale);
}

void const Matrix::operator *= (const float scale)
{
	this->m0 *= scale;	this->m1 *= scale;	this->m2 *= scale;	this->m3 *= scale;	
	this->m4 *= scale;	this->m5 *= scale;	this->m6 *= scale;	this->m7 *= scale;	
	this->m8 *= scale;	this->m9 *= scale;	this->m10 *= scale;	this->m11 *= scale;	
	this->m12 *= scale;	this->m13 *= scale;	this->m14 *= scale;	this->m15 *= scale;
}

//float * matrix operator
Matrix const operator * (const float scale, const Matrix& v)
{
	return Matrix(v[m0] * scale,v[m1] * scale, v[m2] * scale, v[m3] * scale,
					v[m4] * scale, v[m5] * scale, v[m6] * scale, v[m7] * scale,
					v[m8] * scale, v[m9] * scale, v[m10] * scale,v[m11] * scale,
					v[m12] * scale, v[m13] * scale, v[m14] * scale,v[m15] * scale);
}

//comparison functions
bool Matrix::isEqual(const Matrix& t, const float tolerance)
{
	return ((Util::isEqual(this->m0, t.m0, tolerance)) &&
			(Util::isEqual(this->m1, t.m1, tolerance)) &&
			(Util::isEqual(this->m2, t.m2, tolerance)) &&
			(Util::isEqual(this->m3, t.m3, tolerance)) &&
			(Util::isEqual(this->m4, t.m4, tolerance)) &&
			(Util::isEqual(this->m5, t.m5, tolerance)) &&
			(Util::isEqual(this->m6, t.m6, tolerance)) &&
			(Util::isEqual(this->m7, t.m7, tolerance)) &&
			(Util::isEqual(this->m8, t.m8, tolerance)) &&
			(Util::isEqual(this->m9, t.m9, tolerance)) &&
			(Util::isEqual(this->m10, t.m10, tolerance)) &&
			(Util::isEqual(this->m11, t.m11, tolerance)) &&
			(Util::isEqual(this->m12, t.m12, tolerance)) &&
			(Util::isEqual(this->m13, t.m13, tolerance)) &&
			(Util::isEqual(this->m14, t.m14, tolerance)) &&
			(Util::isEqual(this->m15, t.m15, tolerance)));
}
bool Matrix::isIdentity(const float tolerance)
{
	return ((Util::isOne(this->m0, tolerance)) &&
			(Util::isZero(this->m1, tolerance)) &&
			(Util::isZero(this->m2, tolerance)) &&
			(Util::isZero(this->m3, tolerance)) &&
			(Util::isZero(this->m4, tolerance)) &&
			(Util::isOne(this->m5, tolerance)) &&
			(Util::isZero(this->m6, tolerance)) &&
			(Util::isZero(this->m7, tolerance)) &&
			(Util::isZero(this->m8, tolerance)) &&
			(Util::isZero(this->m9, tolerance)) &&
			(Util::isOne(this->m10, tolerance)) &&
			(Util::isZero(this->m11, tolerance)) &&
			(Util::isZero(this->m12, tolerance)) &&
			(Util::isZero(this->m13, tolerance)) &&
			(Util::isZero(this->m14, tolerance)) &&
			(Util::isOne(this->m15, tolerance)));
}

//multiplication
Matrix const Matrix::operator * (const Matrix & t) const
{
	return Matrix(	this->m0 * t.m0 + this->m1 * t.m4 + this->m2 * t.m8 + this->m3 * t.m12,
					this->m0 * t.m1 + this->m1 * t.m5 + this->m2 * t.m9 + this->m3 * t.m13,
					this->m0 * t.m2 + this->m1 * t.m6 + this->m2 * t.m10 + this->m3 * t.m14,
					this->m0 * t.m3 + this->m1 * t.m7 + this->m2 * t.m11 + this->m3 * t.m15,
					
					this->m4 * t.m0 + this->m5 * t.m4 + this->m6 * t.m8 + this->m7 * t.m12,
					this->m4 * t.m1 + this->m5 * t.m5 + this->m6 * t.m9 + this->m7 * t.m13,
					this->m4 * t.m2 + this->m5 * t.m6 + this->m6 * t.m10 + this->m7 * t.m14,
					this->m4 * t.m3 + this->m5 * t.m7 + this->m6 * t.m11 + this->m7 * t.m15,

					this->m8 * t.m0 + this->m9 * t.m4 + this->m10 * t.m8 + this->m11 * t.m12,
					this->m8 * t.m1 + this->m9 * t.m5 + this->m10 * t.m9 + this->m11 * t.m13,
					this->m8 * t.m2 + this->m9 * t.m6 + this->m10 * t.m10 + this->m11 * t.m14,
					this->m8 * t.m3 + this->m9 * t.m7 + this->m10 * t.m11 + this->m11 * t.m15,

					this->m12 * t.m0 + this->m13 * t.m4 + this->m14 * t.m8 + this->m15 * t.m12,
					this->m12 * t.m1 + this->m13 * t.m5 + this->m14 * t.m9 + this->m15 * t.m13,
					this->m12 * t.m2 + this->m13 * t.m6 + this->m14 * t.m10 + this->m15 * t.m14,
					this->m12 * t.m3 + this->m13 * t.m7 + this->m14 * t.m11 + this->m15 * t.m15);
}

void const Matrix::operator *= (const Matrix& t)
{
	*this = Matrix(	this->m0 * t.m0 + this->m1 * t.m4 + this->m2 * t.m8 + this->m3 * t.m12,
					this->m0 * t.m1 + this->m1 * t.m5 + this->m2 * t.m9 + this->m3 * t.m13,
					this->m0 * t.m2 + this->m1 * t.m6 + this->m2 * t.m10 + this->m3 * t.m14,
					this->m0 * t.m3 + this->m1 * t.m7 + this->m2 * t.m11 + this->m3 * t.m15,
					
					this->m4 * t.m0 + this->m5 * t.m4 + this->m6 * t.m8 + this->m7 * t.m12,
					this->m4 * t.m1 + this->m5 * t.m5 + this->m6 * t.m9 + this->m7 * t.m13,
					this->m4 * t.m2 + this->m5 * t.m6 + this->m6 * t.m10 + this->m7 * t.m14,
					this->m4 * t.m3 + this->m5 * t.m7 + this->m6 * t.m11 + this->m7 * t.m15,

					this->m8 * t.m0 + this->m9 * t.m4 + this->m10 * t.m8 + this->m11 * t.m12,
					this->m8 * t.m1 + this->m9 * t.m5 + this->m10 * t.m9 + this->m11 * t.m13,
					this->m8 * t.m2 + this->m9 * t.m6 + this->m10 * t.m10 + this->m11 * t.m14,
					this->m8 * t.m3 + this->m9 * t.m7 + this->m10 * t.m11 + this->m11 * t.m15,

					this->m12 * t.m0 + this->m13 * t.m4 + this->m14 * t.m8 + this->m15 * t.m12,
					this->m12 * t.m1 + this->m13 * t.m5 + this->m14 * t.m9 + this->m15 * t.m13,
					this->m12 * t.m2 + this->m13 * t.m6 + this->m14 * t.m10 + this->m15 * t.m14,
					this->m12 * t.m3 + this->m13 * t.m7 + this->m14 * t.m11 + this->m15 * t.m15);	
}

//determinant
float Matrix::det()
{
	// det = m0 * detA - m1 * detB + m2 * detC - m3 * detD
	
	// detA = m5 * detAA - m6 * detAB + m7 * detAC
	// DetB = m4 * detBA - m6 * detBB + m7 * detBC
	// DetC = m4 * detCA - m5 * detCB + m7 * detCC
	// DetD = m4 * detDA - m5 * detDB + m6 * detDC

	//A: detAA = detBA = (m10 * m15) - (m11 * m14)
	//B: detAB = detCA = (m9 * m15) - (m11 * m13)
	//C: detAC = detDA = (m9 * m14) - (m10 * m13)
	//D: detBB = detCB = (m8 * m15) - (m11 * m12)
	//E: detBC = detDB = (m8 * m14) - (m10 * m12)
	//F: detCC = detDC = (m8 * m13) - (m9 * m12)

	//det = (m0 * m5) * A - (m0 * m6) * B + (m0 * m7) * C
	//	  - (m1 * m4) * A + (m2 * m4) * B - (m3 * m4) * C
	//	  + (m1 * m6) * D - (m1 * m7) * E + (m2 * m7) * F
	//	  - (m2 * m5) * D + (m3 * m5) * E - (m3 * m6) * F

	return (((m0 * m5) - (m1 * m4)) * ((m10 * m15) - (m11 * m14))) 
		 + (((m2 * m4) - (m0 * m6)) * ((m9 * m15) - (m11 * m13))) 
		 + (((m0 * m7) - (m3 * m4)) * ((m9 * m14) - (m10 * m13)))
		 + (((m1 * m6) - (m2 * m5)) * ((m8 * m15) - (m11 * m12))) 
		 + (((m3 * m5) - (m1 * m7)) * ((m8 * m14) - (m10 * m12)))
		 + (((m2 * m7) - (m3 * m6)) * ((m8 * m13) - (m9 *  m12)));
}

//transpose function
void Matrix::T()
{
	//preserve the top half values before replacing
	float tmpM1 = m1;
	float tmpM2 = m2;
	float tmpM3 = m3;
	float tmpM6 = m6;
	float tmpM7 = m7;
	float tmpM11 = m11;

	//set matrix
	this->m1 = m4;		
	this->m2 = m8;
	this->m3 = m12;

	this->m4 = tmpM1;
	this->m6 = m9;
	this->m7 = m13;

	this->m8 = tmpM2;
	this->m9 = tmpM6;
	this->m11 = m14;

	this->m12 = tmpM3;
	this->m13 = tmpM7;
	this->m14 = tmpM11;
}

//returns transpose
Matrix Matrix::getT()
{
	return Matrix(this->m0, this->m4, this->m8, this->m12,
				  this->m1, this->m5, this->m9, this->m13,
				  this->m2, this->m6, this->m10, this->m14,
				  this->m3, this->m7, this->m11, this->m15);
}

//inverts the matrix
void Matrix::inv()
{
	float det = this->det();
	if(Util::isNonZero(det, MATH_TOLERANCE))
	{
		*this = this->Adjugate() * (1/det);
	}
}

//returns inverse
Matrix Matrix::getInv()
{
	Matrix ret;
	float det = this->det();
	if(Util::isNonZero(det, MATH_TOLERANCE))
	{
		ret = this->Adjugate() * (1/det);
	}
	else
	{
		assert(0);
		//not invertible
	}

	return ret;
}

Matrix Matrix::Adjugate()
{
	//Adj = detA -detE detI -detM
	//		-detB detF -detJ detN
	//		detC -detG detK -detO
	//		-detD detH -detL detP
	//where det[i] = determinant of submatrix of i

	//detA = m5 * detAA - m6 * detAB + m7 * detAC
	//detB = m4 * detBA - m6 * detBB + m7 * detBC
	//detC = m4 * detCA - m5 * detCB + m7 * detCC
	//detD = m4 * detDA - m5 * detDB + m6 * detDC
	//detE = m1 * detEA - m2 * detEB + m3 * detEC
	//detF = m0 * detFA - m2 * detFB + m3 * detFC
	//detG = m0 * detGA - m1 * detGB + m3 * detGC
	//detH = m0 * detHA - m1 * detHB + m2 * detHC
	//detI = m1 * detIA - m2 * detIB + m3 * detIC
	//detJ = m0 * detJA - m2 * detJB + m3 * detJC
	//detK = m0 * detKA - m1 * detKB + m3 * detKC
	//detL = m0 * detLA - m1 * detLB + m2 * detLC
	//detM = m1 * detMA - m2 * detMB + m3 * detMC
	//detN = m0 * detNA - m2 * detNB + m3 * detNC
	//detO = m0 * detOA - m1 * detOB + m3 * detOC
	//detP = m0 * detPA - m1 * detPB + m2 * detPC

	//A: detAA = detBA = detEA = detFA = (m10 * m15) - (m11 * m14)
	//B: detAB = detCA = detEB = detGA = (m9 * m15) - (m11 * m13)
	//C: detAC = detDA = detEC = detHA = (m9 * m14) - (m10 * m13)
	//D: detBB = detCB = detFB = detGB = (m8 * m15) - (m11 * m12)
	//E: detBC = detDB = detFC = detHB = (m8 * m14) - (m10 * m12)
	//F: detCC = detDC = detGC = detHC = (m8 * m13) - (m9 * m12)
	//G: detIA = detJA = (m6 * m15) - (m7 * m14)
	//H: detIB = detKA = (m5 * m15) - (m7 * m13)
	//I: detIC = detLA = (m5 * m14) - (m6 * m13)
	//J: detJB = detKB = (m4 * m15) - (m7 * m12)
	//K: detJC = detLB = (m4 * m14) - (m6 * m12)
	//M: detKC = detLC = (m4 * m13) - (m5 * m12)
	//N: detMA = detNA = (m6 * m11) - (m7 * m10)
	//O: detMB = detOA = (m5 * m11) - (m7 * m9)
	//P: detMC = detPA = (m5 * m10) - (m6 * m9)
	//Q: detNB = detOB = (m4 * m11) - (m7 * m8)
	//R: detNC = detPB = (m4 * m10) - (m6 * m8)
	//S: detOC = detPC = (m4 * m9) - (m5 * m8)

	//Adj0 = m5 * ((m10 * m15) - (m11 * m14)) - m6 * ((m9 * m15) - (m11 * m13)) + m7 * ((m9 * m14) - (m10 * m13))
	//Adj1 = -m1 * ((m10 * m15) - (m11 * m14)) + m2 * ((m9 * m15) - (m11 * m13)) - m3 * ((m9 * m14) - (m10 * m13))
	//Adj2 = m1 * ((m6 * m15) - (m7 * m14)) - m2 * ((m5 * m15) - (m7 * m13)) + m3 * ((m5 * m14) - (m6 * m13))
	//Adj3 =  -m1 * ((m6 * m11) - (m7 * m10)) + m2 * ((m5 * m11) - (m7 * m9)) - m3 * ((m5 * m10) - (m6 * m9))
	//Adj4 = -m4 * ((m10 * m15) - (m11 * m14)) + m6 * ((m8 * m15) - (m11 * m12)) - m7 * ((m8 * m14) - (m10 * m12)) 
	//Adj5 = m0 * ((m10 * m15) - (m11 * m14)) - m2 * ((m8 * m15) - (m11 * m12)) + m3 * ((m8 * m14) - (m10 * m12))
	//Adj6 = -m0 * ((m6 * m15) - (m7 * m14)) + m2 * ((m4 * m15) - (m7 * m12)) - m3 * ((m4 * m14) - (m6 * m12))
	//Adj7 = m0 * ((m6 * m11) - (m7 * m10)) - m2 * ((m4 * m11) - (m7 * m8)) + m3 * ((m4 * m10) - (m6 * m8))
	//Adj8 = m4 * ((m9 * m15) - (m11 * m13)) - m5 * ((m8 * m15) - (m11 * m12)) + m7 * ((m8 * m13) - (m9 * m12))
	//Adj9 = -m0 * ((m9 * m15) - (m11 * m13)) + m1 * ((m8 * m15) - (m11 * m12)) - m3 * ((m8 * m13) - (m9 * m12))
	//Adj10 = m0 * ((m5 * m15) - (m7 * m13)) - m1 * ((m4 * m15) - (m7 * m12)) + m3 * ((m4 * m13) - (m5 * m12))
	//Adj11 = -m0 * ((m5 * m11) - (m7 * m9)) + m1 * ((m4 * m11) - (m7 * m8)) - m3 * ((m4 * m9) - (m5 * m8))
	//Adj12 = -m4 * ((m9 * m14) - (m10 * m13)) + m5 * ((m8 * m14) - (m10 * m12)) - m6 * ((m8 * m13) - (m9 * m12))
	//Adj13 = m0 * ((m9 * m14) - (m10 * m13)) - m1 * ((m8 * m14) - (m10 * m12)) + m2 * ((m8 * m13) - (m9 * m12))
	//Adj14 = -m0 * ((m5 * m14) - (m6 * m13)) + m1 * ((m4 * m14) - (m6 * m12)) - m2 * ((m4 * m13) - (m5 * m12))
	//Adj15 = m0 * ((m5 * m10) - (m6 * m9)) - m1 * ((m4 * m10) - (m6 * m8)) + m2 * ((m4 * m9) - (m5 * m8))

	return Matrix(m5 * ((m10 * m15) - (m11 * m14)) - m6 * ((m9 * m15) - (m11 * m13)) + m7 * ((m9 * m14) - (m10 * m13)),
				-m1 * ((m10 * m15) - (m11 * m14)) + m2 * ((m9 * m15) - (m11 * m13)) - m3 * ((m9 * m14) - (m10 * m13)),
				 m1 * ((m6 * m15) - (m7 * m14)) - m2 * ((m5 * m15) - (m7 * m13)) + m3 * ((m5 * m14) - (m6 * m13)),
				-m1 * ((m6 * m11) - (m7 * m10)) + m2 * ((m5 * m11) - (m7 * m9)) - m3 * ((m5 * m10) - (m6 * m9)),
				-m4 * ((m10 * m15) - (m11 * m14)) + m6 * ((m8 * m15) - (m11 * m12)) - m7 * ((m8 * m14) - (m10 * m12)), 
				 m0 * ((m10 * m15) - (m11 * m14)) - m2 * ((m8 * m15) - (m11 * m12)) + m3 * ((m8 * m14) - (m10 * m12)),
				-m0 * ((m6 * m15) - (m7 * m14)) + m2 * ((m4 * m15) - (m7 * m12)) - m3 * ((m4 * m14) - (m6 * m12)),
				 m0 * ((m6 * m11) - (m7 * m10)) - m2 * ((m4 * m11) - (m7 * m8)) + m3 * ((m4 * m10) - (m6 * m8)),
				 m4 * ((m9 * m15) - (m11 * m13)) - m5 * ((m8 * m15) - (m11 * m12)) + m7 * ((m8 * m13) - (m9 * m12)),
				-m0 * ((m9 * m15) - (m11 * m13)) + m1 * ((m8 * m15) - (m11 * m12)) - m3 * ((m8 * m13) - (m9 * m12)),
				 m0 * ((m5 * m15) - (m7 * m13)) - m1 * ((m4 * m15) - (m7 * m12)) + m3 * ((m4 * m13) - (m5 * m12)),
				-m0 * ((m5 * m11) - (m7 * m9)) + m1 * ((m4 * m11) - (m7 * m8)) - m3 * ((m4 * m9) - (m5 * m8)),
				-m4 * ((m9 * m14) - (m10 * m13)) + m5 * ((m8 * m14) - (m10 * m12)) - m6 * ((m8 * m13) - (m9 * m12)),
				 m0 * ((m9 * m14) - (m10 * m13)) - m1 * ((m8 * m14) - (m10 * m12)) + m2 * ((m8 * m13) - (m9 * m12)),
				-m0 * ((m5 * m14) - (m6 * m13)) + m1 * ((m4 * m14) - (m6 * m12)) - m2 * ((m4 * m13) - (m5 * m12)),
				 m0 * ((m5 * m10) - (m6 * m9)) - m1 * ((m4 * m10) - (m6 * m8)) + m2 * ((m4 * m9) - (m5 * m8)));
}