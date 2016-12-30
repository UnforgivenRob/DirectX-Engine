#ifndef FBX_LOADER_H
#define FBX_LOADER_H

#include "Model.h"
//#include "Texture.h"
//#include "Animation.h"
//#include "Skeleton.h"
//
//class GraphicsObject_Skinned;
//
class FBX_Loader
{
public:
	static void LoadSimple( Model_ID modelID, /*Texture_ID textID,*/ const char* filename );
//	static void LoadComplex( Model_ID* modelIDs, int numModels, Texture_ID* textIDs, int numTexts, const char* filename );
//	static void LoadSkinnedSimple( Model_ID modelID, Anim_ID animID, const char* filename, Skeleton* outSkel, Animatable_GameObject* go );
//	static void LoadSkinnedComplex( Model_ID* modelIDs, const int numModels, Texture_ID* textureIDs, const int numTextures,
//									Anim_ID* animIDs, const int numAnims, const char* filename, Skeleton* outSkel, Animatable_GameObject* go, 
//									GraphicsObject_Skinned* graphObj, bool bOnGPU );
};

#endif