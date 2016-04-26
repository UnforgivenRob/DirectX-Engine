/*****************************************************************************/
/*                                                                           */
/* File: QuatApp.cpp                                                         */
/*                                                                           */
/* Quaternioin Application Class                                             */
/*                                                                           */
/*****************************************************************************/

#include "MathEngine.h"
#include "MathApp.h"
#include <math.h>

// do magic
void QuatApp::Slerp( Quat &result, const Quat &source, const Quat &target, const float slerpFactor )
{
	const float epsilon = 0.001f;
	Quat targ = target;
	float f = source.dot(target);
	
	if(f < 0.0f)
	{
		targ = -targ;
		f = -f;
	}

	float targFactor = slerpFactor;
	float sourceFactor = 1.0f - slerpFactor;

	if(f < 1.0f - epsilon)
	{
		float phi = acosf(f);
		float invSinPhi = 1.0f / sinf(phi);
		sourceFactor = sinf(sourceFactor * phi) * invSinPhi;
		targFactor	 = sinf(targFactor * phi) * invSinPhi;
	}

	result = (source * sourceFactor) + (targ * targFactor);
}
    
void QuatApp::SlerpArray( Quat *result, const Quat *source, const Quat *target, const float slerpFactor, const int numQuats )
{
	for( int i = 0; i < numQuats; i++)
	{
		Quat* qOut = result + i;
		const Quat* qSource = source + i;
		const Quat* qTarget = target + i;

		Slerp(*qOut, *qSource, *qTarget, slerpFactor);
	}
}
/***** END of File QuatApp.cpp ***********************************************/
