/****************************************************************************************

   Copyright (C) 2013 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include "DisplayCommon.h"
#include "DisplayAnimation.h"
//rww
#include <assert.h>
#include <vector>
#include "Anim_Package_Headers.h"
#include "File.h"
#include "KeyframeReducer.h"

#if defined (FBXSDK_ENV_MAC)
// disable the “format not a string literal and no format arguments” warning since
// the FBXSDK_printf calls made here are all valid calls and there is no secuity risk
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

#define Skel_File_Ending ".ske.bin"
#define Animation_File_Ending ".ani.bin"

//RWW
void DisplayAnimationRob(FbxAnimLayer* pAnimLayer, FbxNode* pNode, std::vector<Bone_Trans_Data>& bones, FbxTime time, bool isSwitcher = false);
void DisplayChannelsRob(FbxNode* pNode, FbxAnimLayer* pAnimLayer, void (*DisplayCurve) (FbxAnimCurve* pCurve), void (*DisplayListCurve) (FbxAnimCurve* pCurve, FbxProperty* pProperty),
						std::vector<Bone_Trans_Data>& bones, FbxTime time, bool isSwitcher);
FbxAnimEvaluator* AnimEvaluator;

std::vector<Bone_Trans_Data> boneData;
std::vector<int> times;
extern AnimationHeader aHead;
extern std::vector<Hierarchy_Data> hierarchy;
extern std::vector<Bone_Trans_Data> bindPose;

void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode,FileHandle fh, float pct, bool bReduceKeyframes, bool bRemoveTrans, bool isSwitcher = false);
void DisplayCurveKeys(FbxAnimCurve* pCurve);
void DisplayListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty);

void DisplayAnimation(FbxScene* pScene, const char* filename, float pct, bool bReduceKeyframes, const bool bRemoveTrans)
{
	AnimEvaluator = pScene->GetAnimationEvaluator();

	aHead.numAnims =  pScene->GetSrcObjectCount<FbxAnimStack>();

	FileError err;
	FileHandle fh;

	if( bindPose.size() > 0 ) // bindpose only obtained when exporting with bAnimOnly = false
	{
		//compose skel chunk filename
		int len = strlen( filename );
		char* skelFile = new char[len];
		memcpy(skelFile, filename, len );
		char* tmp = strstr(skelFile, Animation_File_Ending);
		strcpy(tmp, Skel_File_Ending);

		printf(skelFile);

		err = File::open( fh, skelFile, FILE_WRITE );
		if(err !=FILE_SUCCESS)
		{
			printf("Invalid Output File\n");
			return;
		}

		Skel_Header skelH;
		skelH.numBones = hierarchy.size();
		err = File::write(fh, &skelH, sizeof(Skel_Header));
		assert(err == FILE_SUCCESS);

		printf ("HHHHHHHHHH %d\n", hierarchy.size());
		for(unsigned int i = 0; i < hierarchy.size(); ++i)
		{
			err = File::write(fh, &hierarchy[i], sizeof(Hierarchy_Data));
			assert(err == FILE_SUCCESS);
		}	

		printf ("BBBBBBBBBB %d\n", bindPose.size());
		for(unsigned int i = 0; i < bindPose.size(); ++i)
		{
			Matrix m = Matrix( SCALE, bindPose[i].sx, bindPose[i].sy, bindPose[i].sz ) *
						Matrix( ROT_XYZ, bindPose[i].rx, bindPose[i].ry, bindPose[i].rz ) *
						Matrix( TRANS, bindPose[i].tx, bindPose[i].ty, bindPose[i].tz );
			err = File::write(fh, &m.getInv(), sizeof(Matrix));
			assert(err == FILE_SUCCESS);
		}	

		err = File::close( fh );
		assert(err == FILE_SUCCESS);
	}
	err = File::open(fh, filename, FILE_WRITE);
	if(err !=FILE_SUCCESS)
	{
		printf("Invalid Output File\n");
		return;
	}

	err = File::write(fh, &aHead, sizeof(AnimationHeader));
	assert(err == FILE_SUCCESS);

    for (int i = 0; i < aHead.numAnims; i++)	//loop through the animation stacks, currently only have 1
    {
        FbxAnimStack* lAnimStack = pScene->GetSrcObject<FbxAnimStack>(i);
		pScene->SetCurrentAnimationStack(lAnimStack);

		//RWW
		printf("Animation Stack Name: %s\n\n", lAnimStack->GetName());

		boneData.clear();
		times.clear();

		DisplayAnimation(lAnimStack, pScene->GetRootNode(), fh, pct, bReduceKeyframes, bRemoveTrans, true);	//call it twice with both values of switcher
		DisplayAnimation(lAnimStack, pScene->GetRootNode(), fh, pct, bReduceKeyframes, bRemoveTrans);
    }

	File::close(fh);
}

void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, FileHandle fh, float pct, bool bReduceKeyframes, bool bRemoveTrans, bool isSwitcher)
{
    int nbAnimLayers = pAnimStack->GetMemberCount<FbxAnimLayer>();
    FbxString lOutputString;

	FbxTime oneFrame;
	oneFrame.SetTime(0 , 0, 0, 1, 0, 0, FbxTime::eFrames30);
	int oneFrameMillis = (int) oneFrame.GetMilliSeconds();

	//RWW
	if(!isSwitcher)
	{
		printf("Number of Anim Layers: %d\n", nbAnimLayers);

		int l = 0;

		FbxAnimLayer* lAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(l);

		//RWW
		printf("Layer %d:\n", l);
		printf("Switcher Value: %d\n", isSwitcher);
		FbxTime time;

		//rww
		int maxMillis = (int)pAnimStack->GetLocalTimeSpan().GetStop().GetMilliSeconds();
		int MaxFrames = (int)maxMillis/oneFrameMillis;

		printf("Frames:%d\n", MaxFrames);

		for(int i = 0; i < MaxFrames; i++)
		{
			char    lTimeString[256];
			time.SetTime(0, 0, 0, i, 0, 0, FbxTime::eFrames30);

			//rww
			times.push_back((int)time.GetMilliSeconds() / oneFrameMillis);

			time.GetTimeString(lTimeString, FbxUShort(256));
			printf("Frame %d: KeyTime %s (%d ms)\n", i, lTimeString, (int)time.GetMilliSeconds());
				
			//RWW
			DisplayAnimationRob(lAnimLayer, pNode, boneData, time, isSwitcher);
		}

		printf("FrameData:\n");
		std::vector<BoneTransform> bones = std::vector<BoneTransform>();
		for(unsigned int cnt = 0; cnt < boneData.size(); cnt++)
		{
			printf("%d: [%f, %f, %f]\n[%f, %f, %f]\n[%f, %f, %f]\n", cnt,	boneData[cnt].tx, boneData[cnt].ty, boneData[cnt].tz,
																			boneData[cnt].rx, boneData[cnt].ry, boneData[cnt].rz,
																			boneData[cnt].sx, boneData[cnt].sy, boneData[cnt].sz);

			BoneTransform bt;

			if(bRemoveTrans && ( cnt % aHead.numBones == 0 ) )
			{
				bt.Trans = Vect( 0.0f, 0.0f, 0.0f );
			}
			else
			{
				bt.Trans = Vect( boneData[cnt].tx, boneData[cnt].ty, boneData[cnt].tz );
			}

			bt.Rot = Quat( ROT_XYZ, boneData[cnt].rx, boneData[cnt].ry, boneData[cnt].rz );
			if( ( strcmp( pAnimStack->GetName(), "Clip_Walk_Cycle" ) == 0 || strcmp( pAnimStack->GetName(), "Clip_Run_Cycle" ) == 0 ) && ( cnt % aHead.numBones == 0 ) )
			{
				//hack to rotate the run and walk cycle clips around to match the direction of the other clips
				bt.Rot *= Quat(ROT_Z, MATH_PI);
			}


			bt.Scale = Vect( boneData[cnt].sx, boneData[cnt].sy, boneData[cnt].sz );
			bones.emplace_back( bt );
		}
		
		if( bReduceKeyframes )
		{
			KeyframeReducer::reduce( times, bones, aHead.numBones, pct );
		}
		
		Anim_Header animHead;
		animHead.maxTime = times[times.size() - 1];
		animHead.numKeyFrames = times.size();
		animHead.totalSize = times.size() * (aHead.numBones * sizeof(BoneTransform_Reduced) + sizeof(int)) + aHead.numBones * sizeof(Bone_Trans_Data);
		strcpy_s(animHead.name, 19, pAnimStack->GetName());

		FileError err;
		err = File::write(fh, &animHead, sizeof(animHead));
		assert(err ==FILE_SUCCESS);

		printf("Times:\n");
		for(unsigned int cnt = 0; cnt < times.size(); cnt++)
		{
			printf("%d\n", times[cnt]);
			err = File::write(fh, &times[cnt], sizeof(int));
			assert(err ==FILE_SUCCESS);
		}

		for(unsigned int cnt = 0; cnt < bones.size(); cnt++)
		{
			BoneTransform_Reduced btr( bones[cnt].Rot, bones[cnt].Trans );
			err = File::write(fh, &btr, sizeof(BoneTransform_Reduced));
			assert(err ==FILE_SUCCESS);
		}
    }
}


void DisplayAnimationRob(FbxAnimLayer* pAnimLayer, FbxNode* pNode, std::vector<Bone_Trans_Data>& bones, FbxTime time, bool isSwitcher)
{
	//RWW
	if(strcmp(pNode->GetTypeName(), "LimbNode") == 0 )
	{
		printf("     Node Name: %s\n", pNode->GetName());

		DisplayChannelsRob(pNode, pAnimLayer, DisplayCurveKeys, DisplayListCurveKeys, bones, time, isSwitcher);
		FBXSDK_printf ("\n");
	}
	
	for(int lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
	{
		//RWW
		DisplayAnimationRob(pAnimLayer, pNode->GetChild(lModelCount), bones, time, isSwitcher);	//displays children
	}
	
}
#define MATH_PI_180 0.0174532925f

void DisplayChannelsRob(FbxNode* pNode, FbxAnimLayer* pAnimLayer, void (*DisplayCurve) (FbxAnimCurve* pCurve), void (*DisplayListCurve) (FbxAnimCurve* pCurve, FbxProperty* pProperty), 
						std::vector<Bone_Trans_Data>& bones, FbxTime time, bool isSwitcher)
{
    FbxAnimCurve* lAnimCurve = NULL;
	FbxTime::SetGlobalTimeMode(FbxTime::eFrames30);
    // Display general curves.
    if (!isSwitcher)
	{		
		Bone_Trans_Data btd;
		lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if(!lAnimCurve)
		{
			lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		}
		if(!lAnimCurve)
		{
			lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		}

		if(!lAnimCurve)
		{
			if(strstr(pNode->GetName(), "Bip01 Pelvis") == 0)
			{
				printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
				FbxAMatrix localMat = pNode->EvaluateLocalTransform( time);

				FbxVector4 trans = localMat.GetT();
				FbxVector4 rot = localMat.GetR();
				FbxVector4 scale = localMat.GetS();

				btd.tx = (float)trans[0];
				btd.ty = (float)trans[1];
				btd.tz = (float)trans[2];
				btd.rx = (float)rot[0] * MATH_PI_180;
				btd.ry = (float)rot[1] * MATH_PI_180;
				btd.rz = (float)rot[2] * MATH_PI_180;
				btd.sx = (float)scale[0];
				btd.sy = (float)scale[1];
				btd.sz = (float)scale[2];
			}
			else
			{
				printf("YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY\n");
				btd.tx = 0.0f;
				btd.ty = 0.0f;
				btd.tz = 0.0f;
				btd.rx = 0.0f;
				btd.ry = 0.0f;
				btd.rz = 0.0f;
				btd.sx = 1.0f;
				btd.sy = 1.0f;
				btd.sz = 1.0f;
			}

			bones.push_back(btd);

			printf("\tTrans: [%f, %f, %f]\n", btd.tx, btd.ty, btd.tz);
			printf("\t  Rot: [%f, %f, %f]\n", btd.rx, btd.ry, btd.rz); 
			printf("\tScale: [%f, %f, %f]\n", btd.sx, btd.sy, btd.sz); 
			return;
		}

		FbxAMatrix localMat = pNode->EvaluateLocalTransform( time);

		FbxVector4 trans = localMat.GetT();
		FbxVector4 rot = localMat.GetR();
		FbxVector4 scale = localMat.GetS();

		btd.tx = (float)trans[0];
		btd.ty = (float)trans[1];
		btd.tz = (float)trans[2];
		btd.rx = (float)rot[0] * MATH_PI_180;
		btd.ry = (float)rot[1] * MATH_PI_180;
		btd.rz = (float)rot[2] * MATH_PI_180;
		btd.sx = (float)scale[0];
		btd.sy = (float)scale[1];
		btd.sz = (float)scale[2];

		bones.push_back(btd);

		printf("\tTrans: [%f, %f, %f]\n", btd.tx, btd.ty, btd.tz); 
		printf("\t  Rot: [%f, %f, %f]\n", btd.rx, btd.ry, btd.rz); 
		printf("\tScale: [%f, %f, %f]\n", btd.sx, btd.sy, btd.sz); 
	}
}


static int InterpolationFlagToIndex(int flags)
{
	if( (flags & FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant ) return 1;
    if( (flags & FbxAnimCurveDef::eInterpolationLinear) == FbxAnimCurveDef::eInterpolationLinear ) return 2;
	if( (flags & FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic ) return 3;
    return 0;
}

static int ConstantmodeFlagToIndex(int flags)
{
    if( (flags & FbxAnimCurveDef::eConstantStandard) == FbxAnimCurveDef::eConstantStandard ) return 1;
    if( (flags & FbxAnimCurveDef::eConstantNext) == FbxAnimCurveDef::eConstantNext ) return 2;
    return 0;
}

static int TangentmodeFlagToIndex(int flags)
{
    if( (flags & FbxAnimCurveDef::eTangentAuto) == FbxAnimCurveDef::eTangentAuto ) return 1;
    if( (flags & FbxAnimCurveDef::eTangentAutoBreak)== FbxAnimCurveDef::eTangentAutoBreak ) return 2;
    if( (flags & FbxAnimCurveDef::eTangentTCB) == FbxAnimCurveDef::eTangentTCB ) return 3;
    if( (flags & FbxAnimCurveDef::eTangentUser) == FbxAnimCurveDef::eTangentUser ) return 4;
    if( (flags & FbxAnimCurveDef::eTangentGenericBreak) == FbxAnimCurveDef::eTangentGenericBreak ) return 5;
    if( (flags & FbxAnimCurveDef::eTangentBreak) == FbxAnimCurveDef::eTangentBreak ) return 6;
    return 0;
}

static int TangentweightFlagToIndex(int flags)
{
    if( (flags & FbxAnimCurveDef::eWeightedNone) == FbxAnimCurveDef::eWeightedNone ) return 1;
    if( (flags & FbxAnimCurveDef::eWeightedRight) == FbxAnimCurveDef::eWeightedRight ) return 2;
    if( (flags & FbxAnimCurveDef::eWeightedNextLeft) == FbxAnimCurveDef::eWeightedNextLeft ) return 3;
    return 0;
}

static int TangentVelocityFlagToIndex(int flags)
{
    if( (flags & FbxAnimCurveDef::eVelocityNone) == FbxAnimCurveDef::eVelocityNone ) return 1;
    if( (flags & FbxAnimCurveDef::eVelocityRight) == FbxAnimCurveDef::eVelocityRight ) return 2;
    if( (flags & FbxAnimCurveDef::eVelocityNextLeft) == FbxAnimCurveDef::eVelocityNextLeft ) return 3;
    return 0;
}

void DisplayCurveKeys(FbxAnimCurve* pCurve)
{
    static const char* interpolation[] = { "?", "constant", "linear", "cubic"};
    static const char* constantMode[] =  { "?", "Standard", "Next" };
    static const char* cubicMode[] =     { "?", "Auto", "Auto break", "Tcb", "User", "Break", "User break" };
    static const char* tangentWVMode[] = { "?", "None", "Right", "Next left" };

    FbxTime   lKeyTime;
    float   lKeyValue;
    char    lTimeString[256];
    FbxString lOutputString;
    int     lCount;

    int lKeyCount = pCurve->KeyGetCount();

    for(lCount = 0; lCount < lKeyCount; lCount++)
    {
        lKeyValue = static_cast<float>(pCurve->KeyGetValue(lCount));
        lKeyTime  = pCurve->KeyGetTime(lCount);

        lOutputString = "            Key Time: ";
        lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
        lOutputString += ".... Key Value: ";
        lOutputString += lKeyValue;
        lOutputString += " [ ";
        lOutputString += interpolation[ InterpolationFlagToIndex(pCurve->KeyGetInterpolation(lCount)) ];
        if ((pCurve->KeyGetInterpolation(lCount)&FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant)
        {
            lOutputString += " | ";
            lOutputString += constantMode[ ConstantmodeFlagToIndex(pCurve->KeyGetConstantMode(lCount)) ];
        }
        else if ((pCurve->KeyGetInterpolation(lCount)&FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic)
        {
            lOutputString += " | ";
            lOutputString += cubicMode[ TangentmodeFlagToIndex(pCurve->KeyGetTangentMode(lCount)) ];
            lOutputString += " | ";
			lOutputString += tangentWVMode[ TangentweightFlagToIndex(pCurve->KeyGet(lCount).GetTangentWeightMode()) ];
            lOutputString += " | ";
			lOutputString += tangentWVMode[ TangentVelocityFlagToIndex(pCurve->KeyGet(lCount).GetTangentVelocityMode()) ];
        }
        lOutputString += " ]";
        lOutputString += "\n";
        FBXSDK_printf (lOutputString);
    }
}

void DisplayListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty)
{
    FbxTime   lKeyTime;
    int     lKeyValue;
    char    lTimeString[256];
    FbxString lListValue;
    FbxString lOutputString;
    int     lCount;

    int lKeyCount = pCurve->KeyGetCount();

    for(lCount = 0; lCount < lKeyCount; lCount++)
    {
        lKeyValue = static_cast<int>(pCurve->KeyGetValue(lCount));
        lKeyTime  = pCurve->KeyGetTime(lCount);

        lOutputString = "            Key Time: ";
        lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
        lOutputString += ".... Key Value: ";
        lOutputString += lKeyValue;
        lOutputString += " (";
        lOutputString += pProperty->GetEnumValue(lKeyValue);
        lOutputString += ")";

        lOutputString += "\n";
        FBXSDK_printf (lOutputString);
    }
}
