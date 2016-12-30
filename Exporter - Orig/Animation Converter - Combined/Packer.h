#ifndef PACKER_H
#define PACKER_H

#include "File.h"
#include "targa.h"
#include "GL\glew.h"
#include "MathEngine.h"
#define MAXSTRINGSIZE 20

enum ChunkType
{
	VBO_TYPE = 0xA0000000,
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

struct VBO_header
{
	int numVerts;
	int numIndices;
	int stride;
};

struct Text_Header
{
	int height;
	int width;
	GLint iComponents;
	GLenum eFormat;
};

class Packer
{
public:
	static void packChunk(FileHandle fh, unsigned char* chunk, ChunkType type, char* chunkName, unsigned int chunkSize);
};

#endif
