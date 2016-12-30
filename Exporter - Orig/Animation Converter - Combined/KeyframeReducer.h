#ifndef KEYFRAMEREDUCER_H
#define KEYFRAMEREDUCER_H

#include <vector>
#include "Anim_Package_Headers.h"

class KeyframeReducer
{
public:
	static bool reduce( std::vector<int>& times, std::vector<BoneTransform>& data, int numBones, float pct );
};

#endif

