/****************************************************************************************

   Copyright (C) 2013 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include "DisplayLink.h"
#include "Anim_Package_Headers.h"
#include "Vert_Structs.h"
#include <vector>
#include "File.h"
#if defined (FBXSDK_ENV_MAC)
// disable the “format not a string literal and no format arguments” warning since
// the FBXSDK_printf calls made here are all valid calls and there is no secuity risk
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

extern std::vector<Hierarchy_Data> hierarchy;
extern std::vector<Vert> verts;
extern std::vector<Vert_Slim> slimVerts;
extern std::vector<Vert_Skinning> skinningVerts;
std::vector<Bone_Trans_Data> bindPose;

void DisplayLink(FbxGeometry* pGeometry, char* filename)
{
    //Display cluster now

    int i, j;
    int lSkinCount=0;
    int lClusterCount=0;
    FbxCluster* lCluster;

	//rww 
	struct weightedLink
	{
		float weight;
		int	  link;
	};

	//create 
	std::vector< std::vector< weightedLink > > weightedLinks;
	weightedLinks.reserve( verts.size() );
	for(unsigned int num = 0; num < verts.size(); ++num )
	{
		weightedLinks.emplace_back( std::vector< weightedLink >() );
	}


	bindPose.reserve(hierarchy.size());
	for(unsigned int num = 0; num < hierarchy.size(); ++num)
	{
		Bone_Trans_Data data;
		data.rx = 0.0f;
		data.ry = 0.0f;
		data.rz = 0.0f;
		data.tx = 0.0f;
		data.ty = 0.0f;
		data.tz = 0.0f;
		data.sx = 1.0f;
		data.sy = 1.0f;
		data.sz = 1.0f;
		bindPose.emplace_back( data );
	}
    lSkinCount=pGeometry->GetDeformerCount(FbxDeformer::eSkin);
	const char* lClusterModes[] = { "Normalize", "Additive", "Total1" };
	printf("START SKINNING DATA_______________________________________________________________%d\n", lSkinCount);
    for(i=0; i!=lSkinCount; ++i)
    {
		printf( "Cluster %d\n", i);
        lClusterCount = ((FbxSkin *) pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetClusterCount();
        for (j = 0; j != lClusterCount; ++j)
        {
            lCluster=((FbxSkin *) pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetCluster(j);
			
            DisplayString("    Mode: ", lClusterModes[lCluster->GetLinkMode()]);

			char* LinkName = "";
            if(lCluster->GetLink() != NULL)
            {
				LinkName = (char *) lCluster->GetLink()->GetName();
               
            }

			int HierIndex = -1;

			for ( unsigned int i = 0; i < hierarchy.size(); ++i )
			{
				if( 0 == strcmp ( LinkName, hierarchy[i].name ) )
				{
					HierIndex = hierarchy[i].index;
					break;
				}
			}

			printf( "Name: %s, Index: %d\n", LinkName, HierIndex );

            int k, lIndexCount = lCluster->GetControlPointIndicesCount();
            int* lIndices = lCluster->GetControlPointIndices();
            double* lWeights = lCluster->GetControlPointWeights();

            for(k = 0; k < lIndexCount; k++)
            {
				weightedLink tmp;
				tmp.weight = (float)lWeights[ k ];
				tmp.link = HierIndex - 1;
				printf( "          Link: %d, Weight: %f\n", lIndices[ k ], tmp.weight);
				weightedLinks[ lIndices[ k ] ].emplace_back( tmp );
            }

            DisplayString("");

            FbxAMatrix lMatrix;

            lMatrix = lCluster->GetTransformMatrix(lMatrix);
            Display3DVector("        Transform Translation: ", lMatrix.GetT());
            Display3DVector("        Transform Rotation: ", lMatrix.GetR());
            Display3DVector("        Transform Scaling: ", lMatrix.GetS());

			FbxAMatrix lMatrix2;
			lMatrix2= lCluster->GetTransformLinkMatrix(lMatrix2);
            Display3DVector("        Transform Link Translation: ", lMatrix2.GetT());
            Display3DVector("        Transform Link Rotation: ", lMatrix2.GetR());
            Display3DVector("        Transform Link Scaling: ", lMatrix2.GetS());

			FbxAMatrix transform = lMatrix.Inverse() * lMatrix2;

			FbxVector4 trans = transform.GetT();
			FbxVector4 rot = transform.GetR();
			FbxVector4 scale = transform.GetS();

			bindPose[HierIndex - 1].tx = (float)trans[0];
			bindPose[HierIndex - 1].ty = (float)trans[1];
			bindPose[HierIndex - 1].tz = (float)trans[2];
			bindPose[HierIndex - 1].rx = (float)rot[0] * MATH_PI_180;
			bindPose[HierIndex - 1].ry = (float)rot[1] * MATH_PI_180;
			bindPose[HierIndex - 1].rz = (float)rot[2] * MATH_PI_180;
			bindPose[HierIndex - 1].sx = (float)scale[0];
			bindPose[HierIndex - 1].sy = (float)scale[1];
			bindPose[HierIndex - 1].sz = (float)scale[2];

            if (lCluster->GetAssociateModel() != NULL)
            {
                lMatrix = lCluster->GetTransformAssociateModelMatrix(lMatrix);
                DisplayString("        Associate Model: ", (char *) lCluster->GetAssociateModel()->GetName());
                Display3DVector("        Associate Model Translation: ", lMatrix.GetT());
                Display3DVector("        Associate Model Rotation: ", lMatrix.GetR());
                Display3DVector("        Associate Model Scaling: ", lMatrix.GetS());
            }

            DisplayString("");
        }
    }

	for(unsigned int num1 = 0; num1 < slimVerts.size(); ++num1 )
	{
		printf("Vert[%d]:\n", num1);
		
		for(unsigned int num2 = 0; num2 < weightedLinks[num1].size(); ++num2)
		{
			weightedLink link = weightedLinks[num1][num2];
			if( num2 < 4 )
			{
				skinningVerts[num1].bones[num2] = link.link;
				skinningVerts[num1].weights[num2] = link.weight;
			}
			else
			{
				//replace lowest value there
				float lowestWeight = 2.0f;
				int lowestIndex = -1;

				for( int i = 0; i < 4; ++i )
				{
					if ( skinningVerts[num1].weights[i] < lowestWeight)
					{
						lowestWeight = skinningVerts[num1].weights[i];
						lowestIndex = i;
					}
				}

				if( link.weight > skinningVerts[num1].weights[lowestIndex])
				{
					skinningVerts[num1].weights[lowestIndex] = link.weight;
					skinningVerts[num1].bones[lowestIndex] = link.link;
				}
			}
			printf("Bone: %d, Weight %f\n", link.link, link.weight);
		}
	}

	for(unsigned int num1 = 0; num1 < skinningVerts.size(); ++num1 )
	{
		Vert_Skinning vert = skinningVerts[num1];
		float totalWeight = vert.weights[0] + vert.weights[1] +	vert.weights[2] + vert.weights[3];
		if(totalWeight < 1.0f )
		{
			skinningVerts[num1].weights[0] /= totalWeight;
			skinningVerts[num1].weights[1] /= totalWeight;
			skinningVerts[num1].weights[2] /= totalWeight;
			skinningVerts[num1].weights[3] /= totalWeight;
		}

		printf("Updated Vert[%d]:\n", num1);
		vert = skinningVerts[num1];
		for(int i = 0; i < 4; ++i)
		{
			printf( "Bone: %d, Weight %f\n", vert.bones[i], vert.weights[i] );
		}
	}
	printf( "END SKINNING DATA________________________________________________________________\n" );
}


