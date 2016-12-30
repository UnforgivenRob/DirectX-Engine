/****************************************************************************************

   Copyright (C) 2013 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

#include <fbxsdk.h>
#include "Anim_Package_Headers.h"
#include <vector>

std::vector<Hierarchy_Data> hierarchy;
extern AnimationHeader aHead;

#if defined (FBXSDK_ENV_MAC)
// disable the “format not a string literal and no format arguments” warning since
// the FBXSDK_printf calls made here are all valid calls and there is no secuity risk
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

// Local functions prototype.
void DisplayHierarchyRob(FbxNode* pNode, int pDepth, int ParentIndex);

void DisplayHierarchy(FbxScene* pScene)
{
    int i;
    FbxNode* lRootNode = pScene->GetRootNode();

    for( i = 0; i < lRootNode->GetChildCount(); i++)
    {
        DisplayHierarchyRob(lRootNode->GetChild(i), 0, 0);
    }

	aHead.numBones = hierarchy.size();
	for(unsigned int j = 0; j < hierarchy.size(); j++)
	{
		printf("[%d]: parent[%d]: name:%s\n", hierarchy[j].index, hierarchy[j].ParentIndex, hierarchy[j].name);
	}
}

void DisplayHierarchyRob(FbxNode* pNode, int pDepth, int ParentIndex)
{
    FbxString lString;
	static int cnt = 1;
    int i;
	int pIndex = 0;

	if(pNode->GetNodeAttribute() && pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		printf("%s\n", pNode->GetTypeName());
	}
	if(strcmp(pNode->GetTypeName(), "LimbNode") == 0)
	{
		for(i = 0; i < pDepth; i++)
		{
			lString += "     ";
		}

		printf("%s[%d]:%s: Parent:%d\n", lString.Buffer(), cnt, pNode->GetName(), ParentIndex);
		Hierarchy_Data data;
		data.index = cnt;
		data.ParentIndex = ParentIndex;
		strcpy_s(data.name, 38, pNode->GetName());

		hierarchy.push_back(data);

		pIndex = cnt;
		cnt++;
	}

		for(i = 0; i < pNode->GetChildCount(); i++)
		{
			DisplayHierarchyRob(pNode->GetChild(i), pDepth + 1, pIndex);
		}

}
