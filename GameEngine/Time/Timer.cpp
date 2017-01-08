#include "Timer.h"
#include <Windows.h>

Timer::Timer()
	: _ticMark( Time( TIME_ZERO ) )
{
}

Timer::~Timer()
{
}

void Timer::tic()
{
	_ticMark = getSystemTime();
}

const Time Timer::toc() const
{
	if (_ticMark == Time(TIME_ZERO))
	{
		return _ticMark;
	}
	else
	{
		return Time(getSystemTime() - _ticMark);
	}	

}

const Time Timer::getSystemTime()
{
	__int64 curCount;
	QueryPerformanceCounter((LARGE_INTEGER*)&curCount);
	return ( (int)curCount * Time::getSecondsPerCount() * Time( TIME_ONE_SECOND ) );
}
