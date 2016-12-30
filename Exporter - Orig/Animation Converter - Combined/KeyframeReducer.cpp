#include "KeyframeReducer.h"

float getError( const BoneTransform& frame1, const BoneTransform& frame2, const BoneTransform& frame3 )
{
	Quat leg1 = frame2.Rot - frame1.Rot;
	Quat leg2 = frame3.Rot - frame1.Rot;

	float cosTheta = leg1.dot(leg2) / ( leg1.mag() * leg2.mag() );
	return abs( acosf(cosTheta / MATH_PI_180) );
}

float calculateError( const std::vector<BoneTransform>& data, int numbones, int index1, int index2, int index3 )
{
	float error = 0.0;
	for( int i = 0; i < numbones; ++i )
	{
		float ret = getError( data[index1 + i], data[index2 + i], data[index3 + i] );
		if( ret > FLT_MIN)
		{
		//	printf("Error: %f\n", ret);
			error += ret;
		}
	}

	return error;
}

bool KeyframeReducer::reduce( std::vector<int>& times, std::vector<BoneTransform>& data, int numBones, float pct )
{
	unsigned int num = (unsigned int)( ( 1.0f - pct ) * times.size() );
	printf( "Total Count: %d\nReducer Count: %d\n", times.size(), num );

	//find the intermediate frames
	int cnt = 1;

	std::vector<int>::iterator itTimes = times.begin();
	std::vector<BoneTransform>::iterator itData = data.begin();

	while( times.size() > num )
	{
		int indexToRemove = -1;
		int timesIndexToRemove = -1;
		float error = FLT_MAX;

		int cnt = 0;
		for( int i = 0; i < data.size() - ( 2 * numBones ); i += numBones )
		{
			float curErr = calculateError( data, numBones, i, i + numBones, i + ( 2 * numBones ) );

			if( curErr < error )
			{
				error = curErr;
				indexToRemove = i + numBones;
				timesIndexToRemove = cnt + 1;
			}

			cnt++;
		}

		for( int j = 0; j < numBones; ++j )
		{
			data.erase( itData + indexToRemove ); // don't add on j as each removal will shift the rest back to itData + indexToRemove

			itData = data.begin();
		}
		times.erase( itTimes + timesIndexToRemove );
		itTimes = times.begin();
		printf("Removing index: %d\n", ( indexToRemove / numBones ) );
	}

	return true;
}