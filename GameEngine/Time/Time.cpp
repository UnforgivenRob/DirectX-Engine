#include "Time.h"
#include <climits>
#include <Windows.h>

float Time::secondsPerCount = -1.0f;

const Time operator*(const float intVal, const Time& inTime )
{
	return Time( (Time::Representation)( intVal * inTime._rawTime ) );
}

const Time operator*(const int intVal, const Time& inTime )
{
	return Time( intVal * inTime._rawTime );
}

Time::Time(const Duration duration)
{
	getSecondsPerCount();
	switch (duration)
	{
	case TIME_ZERO:
		this->_rawTime = 0;
		break;
	case TIME_NTSC_FRAME:
		this->_rawTime = (Time::Representation)( 1.0f / (30.0f * secondsPerCount) );
		break;
	case TIME_NTSC_30_FRAME:
		this->_rawTime = (Time::Representation)( 1.0f / (30.0f * secondsPerCount) );
		break;
	case TIME_PAL_FRAME:
		this->_rawTime = (Time::Representation)( 1.0f / ( 25.0f * secondsPerCount ) );
		break;
	case TIME_ONE_MILLISECOND:
		this->_rawTime = (Time::Representation)( 1.0f / ( 1000.0f * secondsPerCount ) );
		break;
	case TIME_ONE_SECOND:
		this->_rawTime = (Time::Representation)( 1.0f / secondsPerCount );
		break;
	case TIME_ONE_MINUTE:
		this->_rawTime = (Time::Representation)( 60.0f / secondsPerCount );
		break;
	case TIME_ONE_HOUR:
		this->_rawTime = (Time::Representation)( ( 60.0f * 60.0f ) / secondsPerCount );
		break;
	case TIME_MIN:
		this->_rawTime = LLONG_MIN;
		break;
	case TIME_MAX:
		this->_rawTime = LLONG_MAX;
		break;
	case TIME_DWORD:
	default:
		this->_rawTime = 0;
		break;
	}
}

Time::Time(const Representation value )
	: _rawTime(value)
{}

bool Time::operator==(const Time & rhs) const
{
	return this->_rawTime == rhs._rawTime;
}

bool Time::operator!=(const Time & rhs) const
{
	return this->_rawTime != rhs._rawTime;
}

bool Time::operator<(const Time & rhs) const
{
	return this->_rawTime < rhs._rawTime;
}

bool Time::operator<=(const Time & rhs) const
{
	return this->_rawTime <= rhs._rawTime;
}

bool Time::operator>(const Time & rhs) const
{
	return this->_rawTime > rhs._rawTime;
}

bool Time::operator>=(const Time & rhs) const
{
	return this->_rawTime >= rhs._rawTime;
}

const Time Time::operator-() const
{
	return  Time( -1 * this->_rawTime );
}

const Time Time::operator+(const Time & rhs) const
{
	return Time( this->_rawTime + rhs._rawTime );
}

const Time Time::operator-(const Time & rhs) const
{
	return Time(this->_rawTime - rhs._rawTime);
}

Time & Time::operator+=(const Time & rhs)
{
	this->_rawTime += rhs._rawTime;

	return *this;
}

Time & Time::operator-=(const Time & rhs)
{
	this->_rawTime -= rhs._rawTime;

	return *this;
}

const Time Time::operator*( const float floatVal ) const
{
	return Time( (Time::Representation)( this->_rawTime * floatVal ) );
}

const Time Time::operator*( const int intVal ) const
{
	return Time( this->_rawTime * (Time::Representation)intVal );
}

Time & Time::operator*=(const float floatVal)
{
	// TODO: insert return statement here
	this->_rawTime = (Time::Representation)((float)this->_rawTime * floatVal );

	return *this;
}

Time & Time::operator*=(const int intVal)
{
	this->_rawTime *= intVal;

	return *this;
}

float Time::operator/(const Time & denominator) const
{
	return (float)this->_rawTime / (float)denominator._rawTime;
}

const Time Time::operator/(const float denominator) const
{
	return Time( (Time::Representation)( (float)this->_rawTime / denominator ) );
}

const Time Time::operator/(const int denominator) const
{
	return Time( (Time::Representation)( (float)this->_rawTime / (float)denominator ) );
}

Time & Time::operator/=(const float denominator )
{
	this->_rawTime = (Time::Representation)( (float)this->_rawTime / denominator );
	return *this;
}

Time & Time::operator/=(const int denominator )
{
	this->_rawTime = (Time::Representation)((float)this->_rawTime / (float)denominator );
	return *this;
}

int Time::quotient(const Time & numerator, const Time & denominator)
{
	return numerator._rawTime / denominator._rawTime;
}

const Time Time::remainder(const Time & numerator, const Time & denominator)
{
	return Time( numerator._rawTime % denominator._rawTime );
}

float Time::getSecondsPerCount()
{
	if (secondsPerCount < 0)
	{
		__int64 countsPerSecond = 0;
		QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);

		secondsPerCount = 1.0f / (float)countsPerSecond;
	}

	return secondsPerCount;
}
