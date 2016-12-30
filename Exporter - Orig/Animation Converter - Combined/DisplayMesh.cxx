/****************************************************************************************

   Copyright (C) 2013 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/
#include <assert.h>
#include "DisplayMesh.h"

#include "DisplayMaterial.h"
#include "DisplayTexture.h"
#include "DisplayLink.h"
#include "DisplayShape.h"
#include "DisplayCache.h"

#if defined (FBXSDK_ENV_MAC)
// disable the “format not a string literal and no format arguments” warning since
// the FBXSDK_printf calls made here are all valid calls and there is no secuity risk
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

#define MAT_HEADER_LENGTH 200


//RWW
#include "Vert_Structs.h"
extern std::vector<Vert_Slim> slimVerts;
extern std::vector<Vert_Skinning> skinningVerts;
extern std::vector<Vert> verts;
extern std::vector<Poly> indices;

void DisplayControlsPoints(FbxMesh* pMesh);
void DisplayPolygons(FbxMesh* pMesh);
void DisplayTextureMapping(FbxMesh* pMesh);
void DisplayTextureNames( FbxProperty &pProperty, FbxString& pConnectionString );
void DisplayMaterialTextureConnections( FbxSurfaceMaterial* pMaterial, 
                                       char * header, int pMatId, int l );

void DisplayMesh(FbxNode* pNode, char* filename)
{
    FbxMesh* lMesh = (FbxMesh*) pNode->GetNodeAttribute ();

    DisplayMetaDataConnections(lMesh);
    DisplayControlsPoints(lMesh);

    DisplayPolygons(lMesh);
    DisplayTexture(lMesh);			//RWW
	DisplayLink(lMesh, filename);
    DisplayShape(lMesh);
	
	DisplayCache(lMesh);
}


void DisplayControlsPoints(FbxMesh* pMesh)
{
    int i, lControlPointsCount = pMesh->GetControlPointsCount();
    FbxVector4* lControlPoints = pMesh->GetControlPoints();

	//RWW
    printf("Vertices==============================================================================\n");
	printf("Num: %d\n", lControlPointsCount);

	slimVerts.reserve(lControlPointsCount);
	skinningVerts.reserve(lControlPointsCount);

    for (i = 0; i < lControlPointsCount; i++)
    {
		//RWW
		Vert_Slim vert;
		Vert_Skinning vertSkin;

		vert.vx = (float)lControlPoints[i][0];
		vert.vy = (float)lControlPoints[i][1];
		vert.vz = (float)lControlPoints[i][2];
		vertSkin.vx = (float)lControlPoints[i][0];
		vertSkin.vy = (float)lControlPoints[i][1];
		vertSkin.vz = (float)lControlPoints[i][2];
		for( int cnt = 0; cnt < 4; ++cnt )
		{
			vertSkin.bones[cnt] = 0;
			vertSkin.weights[cnt] = 0.0f;
		}

        for (int j = 0; j < pMesh->GetElementNormalCount(); j++)
        {
            FbxGeometryElementNormal* leNormals = pMesh->GetElementNormal( j);
			if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				printf("Control\n");
				if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
				{
					vert.nx = (float)leNormals->GetDirectArray().GetAt(i)[0];
					vert.ny = (float)leNormals->GetDirectArray().GetAt(i)[1];
					vert.nz = (float)leNormals->GetDirectArray().GetAt(i)[2];
					vertSkin.nx = (float)leNormals->GetDirectArray().GetAt(i)[0];
					vertSkin.ny = (float)leNormals->GetDirectArray().GetAt(i)[1];
					vertSkin.nz = (float)leNormals->GetDirectArray().GetAt(i)[2];
				}
			}
			else if(leNormals->GetMappingMode() == FbxGeometryElement::eByPolygon)
			{
				printf("Poly\n");
				if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
				{
					vert.nx = (float)leNormals->GetDirectArray().GetAt(i)[0];
					vert.ny = (float)leNormals->GetDirectArray().GetAt(i)[1];
					vert.nz = (float)leNormals->GetDirectArray().GetAt(i)[2];
					vertSkin.nx = (float)leNormals->GetDirectArray().GetAt(i)[0];
					vertSkin.ny = (float)leNormals->GetDirectArray().GetAt(i)[1];
					vertSkin.nz = (float)leNormals->GetDirectArray().GetAt(i)[2];
				}
			}

			else if(leNormals->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				printf("polyVert\n");
				if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
				{
					vert.nx = (float)leNormals->GetDirectArray().GetAt(i)[0];
					vert.ny = (float)leNormals->GetDirectArray().GetAt(i)[1];
					vert.nz = (float)leNormals->GetDirectArray().GetAt(i)[2];
					vertSkin.nx = (float)leNormals->GetDirectArray().GetAt(i)[0];
					vertSkin.ny = (float)leNormals->GetDirectArray().GetAt(i)[1];
					vertSkin.nz = (float)leNormals->GetDirectArray().GetAt(i)[2];
				}
			}

			else
			{
				//complain so it will be fixed
				assert(0);
			}
        }

		printf("   V[%d]: %f, %f, %f,   %f, %f, %f\n", i, vert.vx, vert.vy, vert.vz, vert.nx, vert.ny, vert.nz);
		slimVerts.push_back(vert);
		skinningVerts.push_back(vertSkin);
    }

    DisplayString("");

	printf("SlimVerts(%d)\n", slimVerts.size());
	for(unsigned int cnt = 0; cnt < slimVerts.size(); cnt++)
	{
		printf("   V[%d]: %f, %f, %f,   %f, %f, %f\n", cnt, slimVerts[cnt].vx, slimVerts[cnt].vy, slimVerts[cnt].vz,
												slimVerts[cnt].nx, slimVerts[cnt].ny, slimVerts[cnt].nz);
	}
}

void DisplayPolygons(FbxMesh* pMesh)
{
    int i, j, lPolygonCount = pMesh->GetPolygonCount();
    FbxVector4* lControlPoints = pMesh->GetControlPoints(); 
    char header[100];

	//RWW
	printf("Polygons==============================================================================\n");
	printf("Num: %d", lPolygonCount);

	verts.reserve( lPolygonCount * 3);
	indices.reserve(lPolygonCount);

	int indexCnt = 0;

    int vertexId = 0;
    for (i = 0; i < lPolygonCount; i++)
    {
		printf("  Polygon[%d]\n", i);

        int lPolygonSize = pMesh->GetPolygonSize(i);

		//RWW double check that we're working with triangles
		assert(lPolygonSize == 3);
		Poly p;

		for (j = 0; j < lPolygonSize; j++)
		{
			int lControlPointIndex = pMesh->GetPolygonVertex(i, j);
			
			//RWW
			Vert fullVert; //RWW
			fullVert.vx = slimVerts[lControlPointIndex].vx;
			fullVert.vy = slimVerts[lControlPointIndex].vy;
			fullVert.vz = slimVerts[lControlPointIndex].vz;
			fullVert.nx = slimVerts[lControlPointIndex].nx;
			fullVert.ny = slimVerts[lControlPointIndex].ny;
			fullVert.nz = slimVerts[lControlPointIndex].nz;
			fullVert.slimIndex = lControlPointIndex;

			printf("        Vert[%d]: %f, %f, %f\n", j,	lControlPoints[lControlPointIndex][0],
															lControlPoints[lControlPointIndex][1],
															lControlPoints[lControlPointIndex][2]);

			//assign index
			if(0 == j)
			{
				p.i1 = indexCnt;
				indexCnt++;
			}
			else if( 1 == j)
			{
				p.i2 = indexCnt;
				indexCnt++;
			}
			else if (2 == j)
			{
				p.i3 = indexCnt;
				indexCnt++;
			}
			else
			{
				assert(false);
			}
			int l;
			for (l = 0; l < pMesh->GetElementUVCount(); ++l)
			{
				FbxGeometryElementUV* leUV = pMesh->GetElementUV( l);

				switch (leUV->GetMappingMode())
				{
				default:
				    break;
				case FbxGeometryElement::eByPolygonVertex:
					{
						int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
						switch (leUV->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
						case FbxGeometryElement::eIndexToDirect:
							{
								//RWW
								fullVert.s = (float)leUV->GetDirectArray().GetAt(lTextureUVIndex)[0];
								fullVert.t = (float)leUV->GetDirectArray().GetAt(lTextureUVIndex)[1];

								printf("        UV[%d]: %f, %f\n\n", j,	leUV->GetDirectArray().GetAt(lTextureUVIndex)[0],
																		leUV->GetDirectArray().GetAt(lTextureUVIndex)[1]);
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
					}
					break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
				case FbxGeometryElement::eByControlPoint:
					break;
				}
			}
			for( l = 0; l < pMesh->GetElementNormalCount(); ++l)
			{
				FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal( l);

				if(leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							fullVert.nx = (float)leNormal->GetDirectArray().GetAt(vertexId)[0];
							fullVert.ny = (float)leNormal->GetDirectArray().GetAt(vertexId)[1];
							fullVert.nz = (float)leNormal->GetDirectArray().GetAt(vertexId)[2];
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = leNormal->GetIndexArray().GetAt(vertexId);
							
							fullVert.nx = (float)leNormal->GetDirectArray().GetAt(id)[0];
							fullVert.ny = (float)leNormal->GetDirectArray().GetAt(id)[1];
							fullVert.nz = (float)leNormal->GetDirectArray().GetAt(id)[2];
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}

			}
			for( l = 0; l < pMesh->GetElementTangentCount(); ++l)
			{
				FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent( l);
				FBXSDK_sprintf(header, 100, "        Tangent: "); 

				if(leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Display3DVector(header, leTangent->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = leTangent->GetIndexArray().GetAt(vertexId);
							Display3DVector(header, leTangent->GetDirectArray().GetAt(id));
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}

			}
			for( l = 0; l < pMesh->GetElementBinormalCount(); ++l)
			{

				FbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal( l);

				FBXSDK_sprintf(header, 100, "        Binormal: "); 
				if(leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leBinormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Display3DVector(header, leBinormal->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = leBinormal->GetIndexArray().GetAt(vertexId);
							Display3DVector(header, leBinormal->GetDirectArray().GetAt(id));
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}
			}
			vertexId++;

			//RWW
			verts.push_back(fullVert);

		} // for polygonSize
		indices.push_back(p);
    } // for polygonCount

	printf("VERTS (from STL):=======================================================\n");
	for(unsigned int cnt = 0; cnt < verts.size(); cnt++)
	{
		printf("V[%d]: %f, %f, %f : %f, %f : %f, %f, %f\n", cnt, verts[cnt].vx, verts[cnt].vy, verts[cnt].vz,
																verts[cnt].s, verts[cnt].t,
																verts[cnt].nx, verts[cnt].ny, verts[cnt].nz);
	}

	printf("Indices (from STL):=======================================================\n");
	for(unsigned int cnt = 0; cnt < indices.size(); cnt++)
	{
		printf("Poly[%d]: %d, %d, %d\n", cnt,	 indices[cnt].i1,
											 indices[cnt].i2,
											 indices[cnt].i3);
	}

    DisplayString("");
}

void DisplayTextureNames( FbxProperty &pProperty, FbxString& pConnectionString )
{
    int lLayeredTextureCount = pProperty.GetSrcObjectCount<FbxLayeredTexture>();
    if(lLayeredTextureCount > 0)
    {
        for(int j=0; j<lLayeredTextureCount; ++j)
        {
            FbxLayeredTexture *lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
            int lNbTextures = lLayeredTexture->GetSrcObjectCount<FbxTexture>();
            pConnectionString += " Texture ";

            for(int k =0; k<lNbTextures; ++k)
            {
                //lConnectionString += k;
                pConnectionString += "\"";
                pConnectionString += (char*)lLayeredTexture->GetName();
                pConnectionString += "\"";
                pConnectionString += " ";
            }
            pConnectionString += "of ";
            pConnectionString += pProperty.GetName();
            pConnectionString += " on layer ";
            pConnectionString += j;
        }
        pConnectionString += " |";
    }
    else
    {
        //no layered texture simply get on the property
        int lNbTextures = pProperty.GetSrcObjectCount<FbxTexture>();

        if(lNbTextures > 0)
        {
            pConnectionString += " Texture ";
            pConnectionString += " ";

            for(int j =0; j<lNbTextures; ++j)
            {
                FbxTexture* lTexture = pProperty.GetSrcObject<FbxTexture>(j);
                if(lTexture)
                {
                    pConnectionString += "\"";
                    pConnectionString += (char*)lTexture->GetName();
                    pConnectionString += "\"";
                    pConnectionString += " ";
                }
            }
            pConnectionString += "of ";
            pConnectionString += pProperty.GetName();
            pConnectionString += " |";
        }
    }
}

void DisplayMaterialTextureConnections( FbxSurfaceMaterial* pMaterial, char * header, int pMatId, int l )
{
    if(!pMaterial)
        return;

    FbxString lConnectionString = "            Material %d -- ";
    //Show all the textures

    FbxProperty lProperty;
    //Diffuse Textures
    lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
    DisplayTextureNames(lProperty, lConnectionString);

    //DiffuseFactor Textures
    lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
    DisplayTextureNames(lProperty, lConnectionString);

    //Emissive Textures
    lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissive);
    DisplayTextureNames(lProperty, lConnectionString);

    //EmissiveFactor Textures
    lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissiveFactor);
    DisplayTextureNames(lProperty, lConnectionString);


    //Ambient Textures
    lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sAmbient);
    DisplayTextureNames(lProperty, lConnectionString); 

    //AmbientFactor Textures
    lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sAmbientFactor);
    DisplayTextureNames(lProperty, lConnectionString);          

    //Specular Textures
    lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sSpecular);
    DisplayTextureNames(lProperty, lConnectionString);  

    //SpecularFactor Textures
    lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sSpecularFactor);
    DisplayTextureNames(lProperty, lConnectionString);

    //Shininess Textures
    lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
    DisplayTextureNames(lProperty, lConnectionString);

    //Bump Textures
    lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sBump);
    DisplayTextureNames(lProperty, lConnectionString);

    //Normal Map Textures
    lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sNormalMap);
    DisplayTextureNames(lProperty, lConnectionString);

    //Transparent Textures
    lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sTransparentColor);
    DisplayTextureNames(lProperty, lConnectionString);

    //TransparencyFactor Textures
    lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
    DisplayTextureNames(lProperty, lConnectionString);

    //Reflection Textures
    lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sReflection);
    DisplayTextureNames(lProperty, lConnectionString);

    //ReflectionFactor Textures
    lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sReflectionFactor);
    DisplayTextureNames(lProperty, lConnectionString);

    //Update header with material info
	bool lStringOverflow = (lConnectionString.GetLen() + 10 >= MAT_HEADER_LENGTH); // allow for string length and some padding for "%d"
	if (lStringOverflow)
	{
		// Truncate string!
		lConnectionString = lConnectionString.Left(MAT_HEADER_LENGTH - 10);
		lConnectionString = lConnectionString + "...";
	}
	FBXSDK_sprintf(header, MAT_HEADER_LENGTH, lConnectionString.Buffer(), pMatId, l);
	DisplayString(header);
}
