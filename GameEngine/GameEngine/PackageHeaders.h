#ifndef PACKAGEHEADERS_H
#define PACKAGEHEADERS_H

#define MAXSTRINGSIZE 20

#include "MathEngine.h"

enum ChunkType
{
	VBO_TYPE = 0xA0000000,
	SKEL_TYPE,
	ANIM_TYPE,
	TEXTURE_TYPE,
	MODELINFO_TYPE
};

struct ChunkHeader
{
	ChunkType type;
	char chunkName[19];
	int chunkSize;
	unsigned int hashNum;
};

struct PackageHeader
{
	char packageName[MAXSTRINGSIZE];
	char versionString[MAXSTRINGSIZE];
	int numChunks;
	int totalSize; // size of file (without package header)
};

struct Model_Info
{
	Vect center;
	float radius;
	int numVBOs;
	int numTexts;
};

struct Skel_Header
{
	int numBones;
};

struct VBO_header
{
	int numVerts;
	int numIndices;
	int stride;
	bool skinnned;
};

//struct Text_Header
//{
//	int height;
//	int width;
//	GLint iComponents;
//	GLenum eFormat;
//};

struct Vert_Final_Skinned
{
	float x;
	float y;
	float z;
	float u;
	float v;
	float nx;
	float ny;
	float nz;
	float weight0;
	float weight1;
	float weight2;
	float weight3;
	int bone0;
	int bone1;
	int bone2;
	int bone3;
};

#endif