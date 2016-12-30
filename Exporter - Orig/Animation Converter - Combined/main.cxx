/****************************************************************************************

   Copyright (C) 2013 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

/////////////////////////////////////////////////////////////////////////
//
// This example illustrates how to detect if a scene is password 
// protected, import and browse the scene to access node and animation 
// information. It displays the content of the FBX file which name is 
// passed as program argument. You can try it with the various FBX files 
// output by the export examples.
//
/////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "DisplayCommon.h"
#include "DisplayHierarchy.h"
#include "DisplayAnimation.h"
#include "DisplayMarker.h"
#include "DisplaySkeleton.h"
#include "DisplayMesh.h"
#include "DisplayNurb.h"
#include "DisplayPatch.h"
#include "DisplayLodGroup.h"
#include "DisplayCamera.h"
#include "DisplayLight.h"
#include "DisplayGlobalSettings.h"
#include "DisplayPose.h"
#include "DisplayPivotsAndLimits.h"
#include "DisplayUserProperties.h"
#include "DisplayGenericInfo.h"
//rww
#include "Vert_Structs.h"
#include "Anim_Package_Headers.h"
#include "File.h"
#include "TGAReader.h"
#include "BoundingSphereCalculator.h"
#include <assert.h>

// Local function prototypes.
void DisplayContent(FbxScene* pScene, char* filename );
void DisplayContent(FbxNode* pNode, char* filename );
void DisplayTarget(FbxNode* pNode);
void DisplayTransformPropagation(FbxNode* pNode);
void DisplayGeometricTransform(FbxNode* pNode);
void DisplayMetaData(FbxScene* pScene);

static bool gVerbose = true;

//defines for the various file extension types
#define VBO_File_Ending ".vbo.bin"
#define Model_Info_Ending ".mod.bin"
#define Skel_File_Ending ".ske.bin"
#define Texture_File_Ending ".tex.bin"
#define Animation_File_Ending ".ani.bin"

// header for animation data
AnimationHeader aHead;

// structs for mesh data
std::vector< Vert_Slim > slimVerts;
std::vector<Vert_Skinning> skinningVerts;
std::vector< Vert > verts;
std::vector< Poly > indices;
std::vector< TGA_Image > textures;
char* textFile = "";

struct ParsingData
{
	bool hasExtra;
	bool bWithSkinning;
	bool bRemoveTrans;
	bool bAnimOnly;
	bool bReduceKeyframes;
	int extraNum;
	int extraIndex;
	float pct;

	ParsingData()
		: hasExtra( false ), bWithSkinning( false ), bRemoveTrans( false ), bAnimOnly( false ), bReduceKeyframes( false ), extraNum( 0 ), extraIndex( 0 ), pct ( 0.0f )
	{
	}
};

void parse( char** input, int current, int max, ParsingData& outData )
{
	if ( strcmp( input[current], "-skinned" ) == 0 || strcmp( input[current], "-Skinned" ) == 0 )
	{
		printf("WITH SKINNING\n");
		outData.bWithSkinning = true;
	}
	else if ( strcmp( input[current], "-removeTrans" ) == 0 || strcmp( input[current], "-RemoveTrans" ) == 0 )
	{
		printf("REMOVE TRANS\n");
		outData.bRemoveTrans = true;
	}
	else if ( strcmp( input[current], "-animOnly" ) == 0 || strcmp( input[current], "-AnimOnly" ) == 0 )
	{
		printf("ANIM ONLY\n");
		outData.bAnimOnly = true;
	}
	else if ( strcmp( input[current], "-reduceKeyframes" ) == 0 ||  strcmp( input[current], "-ReduceKeyframes" ) == 0 )
	{
		printf("REDUCE KEYFRAMES\n");
		outData.bReduceKeyframes = true;
		outData.pct = (float)atof( input[current + 1] );
	}
	else if( strcmp( input[current], "-t") == 0 || strcmp( input[current], "-T") == 0)
	{
		textFile = input[current];
	}
	else if ( strcmp( input[current], "-e") == 0 || strcmp( input[current], "-E") == 0)
	{
		int cnt = atoi( input[current + 1] );

		outData.hasExtra = true;
		outData.extraNum = cnt;
		outData.extraIndex = 8;
	}
}

void parseInput( char** input, int numInput, ParsingData& outData )
{
	for( int i = 3; i < numInput; ++i )
	{
		parse( input, i, numInput, outData );
	}
}

int main(int argc, char** argv)
{
    FbxManager* lSdkManager = NULL;
    FbxScene* lScene = NULL;
    bool lResult;

	ParsingData pd;

	parseInput( argv, argc, pd );

	bool hasExtra = pd.hasExtra;
	bool bWithSkinning = pd.bWithSkinning;
	bool bRemoveTrans = pd.bRemoveTrans;
	bool bAnimOnly = pd.bAnimOnly;
	bool bReduceKeyframes = pd.bReduceKeyframes;
	int extraNum = pd.extraNum;
	int extraIndex = pd.extraIndex;
	float pct = pd.pct;

	//set name in header
	strcpy_s(aHead.name, 19, argv[2]);
	char* filename = new char[strlen(argv[2]) + 8 + 2];
	sprintf(filename,"%s%s", argv[2], ".ani.bin");

	printf("HERE2\n");
    // Prepare the FBX SDK.
    InitializeSdkObjects(lSdkManager, lScene);
    // Load the scene.

    // The example can take a FBX file as an argument.
	FbxString lFilePath = argv[1];

	("Here\n");
	if( lFilePath.IsEmpty() )
	{
        lResult = false;
        FBXSDK_printf("\n\nNo Filename provided\n\n");
	}
	else
	{
		FBXSDK_printf("\n\nFile: %s\n\n", lFilePath.Buffer());
		lResult = LoadScene(lSdkManager, lScene, lFilePath.Buffer());
	}

    if(lResult == false)
    {
        FBXSDK_printf("\n\nAn error occurred while loading the scene...");
		return 0;
    }
    else 
    {
		//RWW - make sure it is converted into triangles
		FbxGeometryConverter lGeomConverter(lSdkManager);
		lGeomConverter.Triangulate(lScene, true);

        // Display the scene.
        DisplayMetaData(lScene);

		//RWW
		if(bAnimOnly)
		{
			FBXSDK_printf("\n\n---------\nHierarchy\n---------\n\n");

			if( gVerbose ) DisplayHierarchy(lScene);

			FBXSDK_printf("\n\n---------\nAnimation\n---------\n\n");

			if( gVerbose ) DisplayAnimation(lScene, filename, pct, bReduceKeyframes, bRemoveTrans);
		}
		else
		{
			FBXSDK_printf("\n\n--------------------\nGlobal Time Settings\n--------------------\n\n");

			if( gVerbose ) DisplayGlobalTimeSettings(&lScene->GetGlobalSettings());

			FBXSDK_printf("\n\n---------\nHierarchy\n---------\n\n");

			if( gVerbose ) DisplayHierarchy(lScene);

			FBXSDK_printf("\n\n------------\nNode Content\n------------\n\n");

			if( gVerbose ) DisplayContent(lScene, filename);

			FBXSDK_printf("\n\n----\nPose\n----\n\n");

			if( gVerbose ) DisplayPose(lScene);

			FBXSDK_printf("\n\n---------\nAnimation\n---------\n\n");

			if( gVerbose ) DisplayAnimation(lScene, filename, pct, bReduceKeyframes, bRemoveTrans);
		}
        //now display generic information
        //FBXSDK_printf("\n\n---------\nGeneric Information\n---------\n\n");
        //if( gVerbose ) DisplayGenericInfo(lScene);
    }

    // Destroy all objects created by the FBX SDK.
    DestroySdkObjects(lSdkManager, lResult);

	if(!bAnimOnly)
	{
		//Reduce vertex buffer
		//copy over 
		std::vector<Vert> verts_to_sort(verts);

		//sort em
		std::sort(verts_to_sort.begin(), verts_to_sort.end());

		//reduce it
		std::vector<Vert> verts_sorted;
		verts_sorted.reserve(verts_to_sort.size());

		Vert tmp;
		Vert cur = verts_to_sort[0];
		verts_sorted.push_back(cur);

		//cycle through all verts
		for(unsigned int i = 0; i < verts_to_sort.size(); ++i)
		{
			tmp = verts_to_sort[i];

			//if different add it and adjust cur
			if( tmp != cur)
			{
				cur = tmp;
				verts_sorted.push_back(tmp);
			}
		}

		std::vector<Vert_Final_Skinned> verts_final_skinned;
		std::vector<Vert_Final> verts_final;

		if( bWithSkinning )
		{
			verts_final_skinned.reserve( verts_sorted.size() );
				
			for( unsigned int i = 0; i < verts_sorted.size(); ++i )
			{
				Vert_Final_Skinned vert;
				vert.x = verts_sorted[i].vx;
				vert.y = verts_sorted[i].vy;
				vert.z = verts_sorted[i].vz;
				vert.u = verts_sorted[i].s;
				vert.v = verts_sorted[i].t;
				vert.nx = verts_sorted[i].nx;
				vert.ny = verts_sorted[i].ny;
				vert.nz = verts_sorted[i].nz;
				vert.weight0 = skinningVerts[verts_sorted[i].slimIndex].weights[0];
				vert.weight1 = skinningVerts[verts_sorted[i].slimIndex].weights[1];
				vert.weight2 = skinningVerts[verts_sorted[i].slimIndex].weights[2];
				vert.weight3 = skinningVerts[verts_sorted[i].slimIndex].weights[3];
				vert.bone0 = skinningVerts[verts_sorted[i].slimIndex].bones[0];
				vert.bone1 = skinningVerts[verts_sorted[i].slimIndex].bones[1];
				vert.bone2 = skinningVerts[verts_sorted[i].slimIndex].bones[2];
				vert.bone3 = skinningVerts[verts_sorted[i].slimIndex].bones[3];

				verts_final_skinned.push_back( vert );
			}
			//Print final Reduced vertex buffer
			printf("Verts_Reduced (%d)\n", verts_final_skinned.size());
			for(unsigned int i = 0; i < verts_final_skinned.size(); ++i)
			{
				printf("V[%d]: %f, %f, %f || %f, %f || %f, %f, %f\n", i, verts_final_skinned[i].x, verts_final_skinned[i].y, verts_final_skinned[i].z,
																		 verts_final_skinned[i].u, verts_final_skinned[i].v,
																		 verts_final_skinned[i].nx, verts_final_skinned[i].ny, verts_final_skinned[i].nz);
				printf( "     Bone:%d, Weight:%f\n     Bone:%d, Weight:%f\n     Bone:%d, Weight:%f\n     Bone:%d, Weight:%f\n", verts_final_skinned[i].bone0, verts_final_skinned[i].weight0,
																																verts_final_skinned[i].bone1, verts_final_skinned[i].weight1,
																																verts_final_skinned[i].bone2, verts_final_skinned[i].weight2,
																																verts_final_skinned[i].bone3, verts_final_skinned[i].weight3 );
			}
		}
		else
		{
			verts_final.reserve( verts_sorted.size() );
			for( unsigned int i = 0; i < verts_sorted.size(); ++i )
			{
				Vert_Final vert;
				vert.x = verts_sorted[i].vx;
				vert.y = verts_sorted[i].vy;
				vert.z = verts_sorted[i].vz;
				vert.u = verts_sorted[i].s;
				vert.v = verts_sorted[i].t;
				vert.nx = verts_sorted[i].nx;
				vert.ny = verts_sorted[i].ny;
				vert.nz = verts_sorted[i].nz;

				verts_final.push_back( vert );
			}
			//Print final Reduced vertex buffer
			printf("Verts_Reduced (%d)\n", verts_final.size());
			for(unsigned int i = 0; i < verts_final.size(); ++i)
			{
				printf("V[%d]: %f, %f, %f || %f, %f || %f, %f, %f\n", i, verts_final[i].x, verts_final[i].y, verts_final[i].z,
																	 verts_final[i].u, verts_final[i].v,
																	 verts_final[i].nx, verts_final[i].ny, verts_final[i].nz);
			}
		}

		//correct indices
		std::vector<Poly> indices_final;
		indices_final.reserve(indices.size());

		std::vector<Vert>::iterator it;	

		Poly oldPoly;
		for(unsigned int i = 0; i < indices.size(); ++i)
		{
			oldPoly = indices[i];
			Poly newPoly;

			Vert vert = verts[oldPoly.i1];
			it = std::find(verts_sorted.begin(), verts_sorted.end(), vert);
			newPoly.i1 = std::distance(verts_sorted.begin(), it);

			vert = verts[oldPoly.i2];
			it = std::find(verts_sorted.begin(), verts_sorted.end(), vert);
			newPoly.i2 = std::distance(verts_sorted.begin(), it);

			vert = verts[oldPoly.i3];
			it = std::find(verts_sorted.begin(), verts_sorted.end(), vert);
			newPoly.i3 = std::distance(verts_sorted.begin(), it);

			indices_final.push_back(newPoly);
		
		}

		printf("Indices_Final:=======================================================\n");
		for(unsigned int cnt = 0; cnt < indices.size(); ++cnt)
		{
			printf("Poly[%d]: %d, %d, %d\n", cnt,	 indices_final[cnt].i1,
													 indices_final[cnt].i2,
													 indices_final[cnt].i3);
		}

		char* fileName = new char[strlen(argv[2]) + 8 + 2];
		Sphere sphereRes;
		FileError res;
		FileHandle fh;

		if( bWithSkinning )
		{
			Vert_Final_Skinned* vertBuff = new Vert_Final_Skinned[verts_final_skinned.size()];
			int* indexBuff = new int[indices_final.size() * 3];
	
			int fullsize = sizeof(VBO_header);
			fullsize += verts_final_skinned.size() * sizeof(Vert_Final_Skinned);
			fullsize += indices_final.size() * 3 * sizeof(int);
	
			for(unsigned int i = 0; i < verts_final_skinned.size(); ++i)
			{
				vertBuff[i].x = verts_final_skinned[i].x;
				vertBuff[i].y = verts_final_skinned[i].y;
				vertBuff[i].z = verts_final_skinned[i].z;
				vertBuff[i].u = verts_final_skinned[i].u;
				vertBuff[i].v = verts_final_skinned[i].v;
				vertBuff[i].nx = verts_final_skinned[i].nx;
				vertBuff[i].ny = verts_final_skinned[i].ny;
				vertBuff[i].nz = verts_final_skinned[i].nz;
				vertBuff[i].weight0 = verts_final_skinned[i].weight0;
				vertBuff[i].weight1 = verts_final_skinned[i].weight1;
				vertBuff[i].weight2 = verts_final_skinned[i].weight2;
				vertBuff[i].weight3 = verts_final_skinned[i].weight3;
				vertBuff[i].bone0 = verts_final_skinned[i].bone0;
				vertBuff[i].bone1 = verts_final_skinned[i].bone1;
				vertBuff[i].bone2 = verts_final_skinned[i].bone2;
				vertBuff[i].bone3 = verts_final_skinned[i].bone3;
			}

			//Calculate bounding sphere
			BoundingSphereCalculator::CalculateSphere(sphereRes, vertBuff, verts_final_skinned.size());

			VBO_header vboHead;
			vboHead.numVerts = verts_final_skinned.size();
			vboHead.numIndices = indices_final.size() * 3;
			vboHead.stride = sizeof(Vert_Final_Skinned);
			vboHead.skinnned = true;

			printf("Center: %f, %f, %f Radius: %f\n", sphereRes.center[x], sphereRes.center[y], sphereRes.center[z], sphereRes.radius);

			for(unsigned int i = 0; i < indices_final.size(); ++i)
			{
				indexBuff[ i * 3] = indices_final[i].i1;
				indexBuff[ i * 3 + 1] = indices_final[i].i2;
				indexBuff[ i * 3 + 2] = indices_final[i].i3;
			}

			unsigned char* vboBuff = new unsigned char[fullsize];
			memcpy(vboBuff, &vboHead, sizeof(VBO_header));
			memcpy(vboBuff + sizeof(VBO_header), vertBuff, verts_final_skinned.size() * sizeof(Vert_Final_Skinned));
			memcpy(vboBuff + sizeof(VBO_header) + verts_final_skinned.size() * sizeof(Vert_Final_Skinned), indexBuff, indices_final.size() * 3 * sizeof(int));
	
			sprintf(fileName,"%s%s", argv[2], VBO_File_Ending);

			File::open(fh, fileName, FileMode::FILE_WRITE);

			res = File::write(fh, vboBuff, fullsize);
			assert(res == FILE_SUCCESS);

			File::close(fh);
		}
		else
		{
			float* vertBuff = new float[verts_final.size() * sizeof(Vert_Final)];
			int* indexBuff = new int[indices_final.size() * 3];
	
			int fullsize = sizeof(VBO_header);
			fullsize += verts_final.size() * sizeof(Vert_Final);
			fullsize += indices_final.size() * 3 * sizeof(int);
	
			for(unsigned int i = 0; i < verts_final.size(); ++i)
			{
				vertBuff[i * 8] = verts_final[i].x;
				vertBuff[i * 8 + 1] = verts_final[i].y;
				vertBuff[i * 8 + 2] = verts_final[i].z;
				vertBuff[i * 8 + 3] = verts_final[i].u;
				vertBuff[i * 8 + 4] = verts_final[i].v;
				vertBuff[i * 8 + 5] = verts_final[i].nx;
				vertBuff[i * 8 + 6] = verts_final[i].ny;
				vertBuff[i * 8 + 7] = verts_final[i].nz;
			}

			//Calculate bounding sphere
			BoundingSphereCalculator::CalculateSphere(sphereRes, (Vert_Final*)vertBuff, verts_final.size());

			VBO_header vboHead;
			vboHead.numVerts = verts_final.size();
			vboHead.numIndices = indices_final.size() * 3;
			vboHead.stride = 8 * sizeof(float);
			vboHead.skinnned = false;

			printf("Center: %f, %f, %f Radius: %f\n", sphereRes.center[x], sphereRes.center[y], sphereRes.center[z], sphereRes.radius);

			for(unsigned int i = 0; i < indices_final.size(); ++i)
			{
				indexBuff[ i * 3] = indices_final[i].i1;
				indexBuff[ i * 3 + 1] = indices_final[i].i2;
				indexBuff[ i * 3 + 2] = indices_final[i].i3;

			}

			unsigned char* vboBuff = new unsigned char[fullsize];
			memcpy(vboBuff, &vboHead, sizeof(VBO_header));
			memcpy(vboBuff + sizeof(VBO_header), vertBuff, verts_final.size() * 8 * sizeof(float));
			memcpy(vboBuff + sizeof(VBO_header) + verts_final.size() * 8 * sizeof(float), indexBuff, indices_final.size() * 3 * sizeof(int));
	
	
			sprintf(fileName,"%s%s", argv[2], VBO_File_Ending);

			File::open(fh, fileName, FileMode::FILE_WRITE);

			res = File::write(fh, vboBuff, fullsize);
			assert(res == FILE_SUCCESS);

			File::close(fh);
		}

		Model_Info mInfo;

		mInfo.center = sphereRes.center;
		mInfo.radius = sphereRes.radius;
		mInfo.numVBOs = 1;		//change later for multi vbo models
		mInfo.numTexts = textures.size() + extraNum;
		
		sprintf(fileName,"%s%s", argv[2], Model_Info_Ending);

		res = File::open(fh, fileName, FileMode::FILE_WRITE);
		assert(res == FILE_SUCCESS);

		res = File::write(fh, &mInfo, sizeof(Model_Info));
		assert(res == FILE_SUCCESS);

		res = File::close(fh);
		assert(res == FILE_SUCCESS);

		//load extra textures
		if(hasExtra)
		{
			printf("EXTRA!!!!!!!!!!");
			for(int i = extraIndex; i < extraIndex + extraNum; i++)
			{
				TGA_Image image = TGAReader::readTGA(argv[i], "");
				textures.push_back(image);
			}
		}

		char* tName = "TEXT_0";

		for(unsigned int j = 0; j < textures.size(); j++)
		{
			Text_Header tHead;
			TGA_Image tga = textures[j];
					printf("HEREXXXXXXXXXXXX\n");
			tHead.width = tga.width;
			tHead.height = tga.height;
			tHead.eFormat = tga.eFormat;
			tHead.iComponents = tga.iComponents;

			long int totalSize = tga.size + sizeof(Text_Header);
			printf("%d", tga.size);
			unsigned char* textBuff = new unsigned char[totalSize];
			memcpy(textBuff, &tHead, sizeof(Text_Header));
			memcpy(textBuff + sizeof(Text_Header), tga.bits, tga.size);
					printf("HEREXXXXXXXXXXXX\n");
			printf("%d\n", totalSize);

			sprintf(fileName, "%s%d%s", argv[2], j, Texture_File_Ending);
			printf(fileName);

			File::open(fh, fileName, FileMode::FILE_WRITE);
			assert(res == FILE_SUCCESS);
		
			File::write(fh, textBuff, totalSize);
			assert(res == FILE_SUCCESS);
					printf("HEREXXXXXXXXXXXX");
			File::close(fh);
			assert(res == FILE_SUCCESS);

			delete[] textBuff;
		}

		delete[] fileName;
	}

    return 0;
}

void DisplayContent( FbxScene* pScene, char* filename )
{
    int i;
    FbxNode* lNode = pScene->GetRootNode();

    if(lNode)
    {
        for(i = 0; i < lNode->GetChildCount(); i++)
        {
			DisplayContent(lNode->GetChild(i), filename );
        }
    }
}

void DisplayContent( FbxNode* pNode, char* filename )
{
    FbxNodeAttribute::EType lAttributeType;
    int i;

    if(pNode->GetNodeAttribute() == NULL)
    {
        FBXSDK_printf("NULL Node Attribute\n\n");
    }
    else
    {
        lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());

        switch (lAttributeType)
        {
	    default:
	        break;
        case FbxNodeAttribute::eMarker:  
            DisplayMarker(pNode);
            break;

        case FbxNodeAttribute::eSkeleton:  
            DisplaySkeleton(pNode);
            break;

        case FbxNodeAttribute::eMesh:      
			DisplayMesh(pNode, filename);
            break;

        case FbxNodeAttribute::eNurbs:      
            //DisplayNurb(pNode);
            break;

        case FbxNodeAttribute::ePatch:     
          //  DisplayPatch(pNode);
            break;

        case FbxNodeAttribute::eCamera:    
           // DisplayCamera(pNode);
            break;

        case FbxNodeAttribute::eLight:     
           // DisplayLight(pNode);
            break;

        case FbxNodeAttribute::eLODGroup:
            //DisplayLodGroup(pNode);
            break;
        }   
    }

    DisplayUserProperties(pNode);
    DisplayTarget(pNode);
    DisplayPivotsAndLimits(pNode);
    DisplayTransformPropagation(pNode);
    DisplayGeometricTransform(pNode);

    for(i = 0; i < pNode->GetChildCount(); i++)
    {
		DisplayContent(pNode->GetChild(i), filename);
    }
}


void DisplayTarget(FbxNode* pNode)
{
    if(pNode->GetTarget() != NULL)
    {
        DisplayString("    Target Name: ", (char *) pNode->GetTarget()->GetName());
    }
}

void DisplayTransformPropagation(FbxNode* pNode)
{
    FBXSDK_printf("    Transformation Propagation\n");

    // 
    // Rotation Space
    //
    EFbxRotationOrder lRotationOrder;
    pNode->GetRotationOrder(FbxNode::eSourcePivot, lRotationOrder);

    FBXSDK_printf("        Rotation Space: ");

    switch (lRotationOrder)
    {
    case eEulerXYZ: 
        FBXSDK_printf("Euler XYZ\n");
        break;
    case eEulerXZY:
        FBXSDK_printf("Euler XZY\n");
        break;
    case eEulerYZX:
        FBXSDK_printf("Euler YZX\n");
        break;
    case eEulerYXZ:
        FBXSDK_printf("Euler YXZ\n");
        break;
    case eEulerZXY:
        FBXSDK_printf("Euler ZXY\n");
        break;
    case eEulerZYX:
        FBXSDK_printf("Euler ZYX\n");
        break;
    case eSphericXYZ:
        FBXSDK_printf("Spheric XYZ\n");
        break;
    }

    //
    // Use the Rotation space only for the limits
    // (keep using eEulerXYZ for the rest)
    //
    FBXSDK_printf("        Use the Rotation Space for Limit specification only: %s\n",
        pNode->GetUseRotationSpaceForLimitOnly(FbxNode::eSourcePivot) ? "Yes" : "No");


    //
    // Inherit Type
    //
    FbxTransform::EInheritType lInheritType;
    pNode->GetTransformationInheritType(lInheritType);

    FBXSDK_printf("        Transformation Inheritance: ");

    switch (lInheritType)
    {
    case FbxTransform::eInheritRrSs:
        FBXSDK_printf("RrSs\n");
        break;
    case FbxTransform::eInheritRSrs:
        FBXSDK_printf("RSrs\n");
        break;
    case FbxTransform::eInheritRrs:
        FBXSDK_printf("Rrs\n");
        break;
    }
}

void DisplayGeometricTransform(FbxNode* pNode)
{
    FbxVector4 lTmpVector;

    FBXSDK_printf("    Geometric Transformations\n");

    //
    // Translation
    //
    lTmpVector = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
    FBXSDK_printf("        Translation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    //
    // Rotation
    //
    lTmpVector = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
    FBXSDK_printf("        Rotation:    %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    //
    // Scaling
    //
    lTmpVector = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
    FBXSDK_printf("        Scaling:     %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);
}


void DisplayMetaData(FbxScene* pScene)
{
    FbxDocumentInfo* sceneInfo = pScene->GetSceneInfo();
    if (sceneInfo)
    {
        FBXSDK_printf("\n\n--------------------\nMeta-Data\n--------------------\n\n");
        FBXSDK_printf("    Title: %s\n", sceneInfo->mTitle.Buffer());
        FBXSDK_printf("    Subject: %s\n", sceneInfo->mSubject.Buffer());
        FBXSDK_printf("    Author: %s\n", sceneInfo->mAuthor.Buffer());
        FBXSDK_printf("    Keywords: %s\n", sceneInfo->mKeywords.Buffer());
        FBXSDK_printf("    Revision: %s\n", sceneInfo->mRevision.Buffer());
        FBXSDK_printf("    Comment: %s\n", sceneInfo->mComment.Buffer());

        FbxThumbnail* thumbnail = sceneInfo->GetSceneThumbnail();
        if (thumbnail)
        {
            FBXSDK_printf("    Thumbnail:\n");

            switch (thumbnail->GetDataFormat())
            {
            case FbxThumbnail::eRGB_24:
                FBXSDK_printf("        Format: RGB\n");
                break;
            case FbxThumbnail::eRGBA_32:
                FBXSDK_printf("        Format: RGBA\n");
                break;
            }

            switch (thumbnail->GetSize())
            {
	        default:
	            break;
            case FbxThumbnail::eNotSet:
                FBXSDK_printf("        Size: no dimensions specified (%ld bytes)\n", thumbnail->GetSizeInBytes());
                break;
            case FbxThumbnail::e64x64:
                FBXSDK_printf("        Size: 64 x 64 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
                break;
            case FbxThumbnail::e128x128:
                FBXSDK_printf("        Size: 128 x 128 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
            }
        }
    }
}

