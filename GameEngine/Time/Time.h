#ifndef ENGINE_TIME_H
#define ENGINE_TIME_H

enum Duration
{
	TIME_ZERO,				// For constructing a Time of zero.
	TIME_NTSC_FRAME,		// For constructing one NTSC 60 hz frame of Time.
	TIME_NTSC_30_FRAME,		// For constructing one NTSC 60 hz frame of Time.		
	TIME_PAL_FRAME,			// For constructing one PAL 50 hz frame of Time.
	TIME_ONE_MILLISECOND,	// For constructing one millisecond of Time.
	TIME_ONE_SECOND,		// For constructing one second of Time.
	TIME_ONE_MINUTE,		// For constructing one minute of Time.
	TIME_ONE_HOUR,			// For constructing one hour of Time.
	TIME_MIN,				// For constructing the most <i>negative</i> Time that can be represented.
	TIME_MAX,				// For constructing the most positive Time that can be represented.
	TIME_DWORD = 0x7FFFFFFF // ensure the enum is size int
};

class Time
{

public:

	//-----------------------------------------------------------------
	// Friends
	//-----------------------------------------------------------------

	/*
	* return: The product of the specified float and Time.
	*/
	friend const Time operator*(const float floatVal, const Time& inTime);

	/*
	* return: The product of the specified integer and Time.
	*/
	friend const Time operator*(const int intVal, const Time& inTime);

	//-----------------------------------------------------------------
	// Constructors / Destructors / Assignment
	//-----------------------------------------------------------------

	// default, same as Time(ZERO)
	inline Time(); 

	// Creates time based on provided duration
	explicit Time(const Duration);

	// Copy Constructor
	inline Time(const Time&);

	// Assignment operator
	inline Time& operator=(const Time& rhs);

	// destrutor
	~Time()
	{}

	//-----------------------------------------------------------------
	// Comparisons
	//-----------------------------------------------------------------

	/*
	* return: true if this Time is equal to the right-hand side.
	*/
	bool operator==(const Time& rhs) const;

	/*
	* return: true if this Time is not equal to the right-hand side.
	*/
	bool operator!=(const Time& rhs) const;

	/*
	* return: true if this Time is less than the right-hand side.
	*/
	bool operator<(const Time& rhs) const;

	/*
	* return: true if this Time is less or equal to the right-hand side.
	*/
	bool operator<=(const Time& rhs) const;

	/*
	* return: true if this Time is greater than the right-hand side.
	*/
	bool operator>(const Time& rhs) const;

	/*
	* return: true if this Time is greather than or equal to
	*  the right-hand side.
	*/
	bool operator>=(const Time& rhs) const;

	//-----------------------------------------------------------------
	// Negation / Addition / Subtraction
	//-----------------------------------------------------------------

	/*
	* return: A negative copy of this Time.
	*/
	const Time operator-() const;

	/*
	* return: The sum of this Time and the right-hand side.
	*/
	const Time operator+(const Time& rhs) const;

	/*
	* return: The difference between this Time and the right-hand side.
	*/
	const Time operator-(const Time& rhs) const;

	/*
	* Adds the right-hand side to this Time.
	*
	* return: This Time.
	*/
	Time& operator+=(const Time& rhs);

	/*
	* Subtracts the right-hand side from this Time.
	*
	* return: This Time.
	*/
	Time& operator-=(const Time& rhs);

	//-----------------------------------------------------------------
	// Multiplication
	//-----------------------------------------------------------------

	/*
	* return: The product of this Time and the specified float.
	*/
	const Time operator*(const float floatVal) const;

	/*
	* return: The product of this Time and the specified integer.
	*/
	const Time operator*(const int intVal) const;

	/*
	* Multiplies this Time by the specified float.
	*
	* return: This Time.
	*/
	Time& operator*=(const float floatVal);

	/*
	* Multiplies this Time by the specified integer.
	*
	* return: This Time.
	*/
	Time& operator*=(const int intVal);

	//-----------------------------------------------------------------
	// name: Division
	//-----------------------------------------------------------------

	/*
	* return: The ratio of this Time divided by the denominator Time.
	*
	* note: Dividing by Time::ZERO is undefined.
	*/
	float operator/(const Time& denominator) const;

	/*
	* return: The result of dividing this Time and by the
	* specified float.
	*
	* note: Dividing by zero is undefined.
	*/
	const Time operator/(const float denominator) const;

	/*
	* return: The result of dividing this Time and by the
	* specified integer.
	*
	* note: Dividing by zero is undefined.
	*/
	const Time operator/(const int denominator) const;

	/*
	* Divides this Time by the specified float.
	*
	* return: This Time.
	*
	* note: Dividing by zero is undefined.
	*/
	Time& operator/=(const float denominator);

	/*
	* Divides this Time by the specified integer.
	*
	* return: This Time.
	*
	* note: Dividing by zero is undefined.
	*/
	Time& operator/=(const int denominator);

	//-----------------------------------------------------------------
	// Quotient / Remainder
	//-----------------------------------------------------------------

	/*
	* return: The <i>truncated</i>, integral division of the
	* numerator and denominator.  In other words,
	* the result of the division is rounded down to the nearest
	* integer.
	*
	* note: Dividing by Time(ZERO) is undefined.
	*/
	static int quotient(const Time& numerator, const Time& denominator);

	/*
	* return: The remainder of dividing the numerator into the
	* denominator.  This is the equivalent of a modulus operation.
	*
	* note: Dividing by Time(ZERO) is undefined.
	*/
	static const Time remainder(const Time& numerator, const Time& denominator);

	static float getSecondsPerCount();

private:
	// types
	typedef int Representation;

	// constructor
	explicit Time(const Representation value);

	// data
	Representation _rawTime;

	static float secondsPerCount;
};

//---------------------------------------------------------------------------
// INLINED IMPLEMENTATION:
//---------------------------------------------------------------------------
Time::Time() 
	: _rawTime(0)
{
}

Time::Time(const Time& rhs)
	: _rawTime(rhs._rawTime)
{
}

Time& Time::operator=(const Time& rhs)
{
	if (this != &rhs)
	{
		this->_rawTime = rhs._rawTime;
	}

	return(*this);
}

#endif  


