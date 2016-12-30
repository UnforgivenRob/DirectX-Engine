#ifndef ANIM_PACKAGE_HEADERS
#define ANIM_PACKAGE_HEADERS

// Mesh structs
#include "MathEngine.h"
#include "GL\glew.h"
#define MAXSTRINGSIZE 20

struct AnimationHeader
{
	int numAnims;
	int numBones;
	int fps;
	char name[20];
};

struct Hierarchy_Data
{
	int index;
	int ParentIndex;
	char name[40];
};

struct Anim_Header
{
	int numKeyFrames;
	int maxTime;
	int totalSize;
	char name[20];
};


struct BoneTransform
{
	Vect Scale;
	Quat   Rot;
	Vect Trans;
};

struct BoneTransform_Reduced
{
	Quat Rot;
	Vect Trans;

	BoneTransform_Reduced( Quat& inRot, Vect& inTrans )
	{
		Rot = inRot;
		Trans = inTrans;
	}
};

struct Bone_Trans_Data
{
	float tx;
	float ty;
	float tz;
	float rx;
	float ry;
	float rz;
	float sx;
	float sy;
	float sz;
};

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

struct Text_Header
{
	int height;
	int width;
	GLint iComponents;
	GLenum eFormat;
};
#endif