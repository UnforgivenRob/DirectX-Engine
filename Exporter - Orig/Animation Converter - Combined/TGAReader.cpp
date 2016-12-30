#include "TGAReader.h"
//#include "targa.h"
#include <memory>
#include <assert.h>
#include "GL\glew.h"
#include "GL\glut.h"

extern char* textFile;

TGA_Image TGAReader::readTGA( const char *filename, const char* name)
{
	FILE *pFile;			// File pointer
	TGAHEADER tgaHeader;		// TGA file header
	unsigned long lImageSize;		// Size in bytes of image
	short sDepth;			// Pixel depth;
	GLbyte	*pBits = NULL;          // Pointer to bits
	TGA_Image ret;
    
	// Default/Failed values
	ret.width = 0;
	ret.height = 0;
	ret.eFormat = GL_RGB;
	ret.iComponents = GL_RGB;
    
	// Attempt to open the file
	char* unConstFilename = new char[1024];
	strcpy(unConstFilename,filename);
	char* slash = "\\";
	char* nextTok;
	char* curTok = strtok_s( unConstFilename, slash, &nextTok); 

	while ( 0 != curTok )
	{
		curTok = strtok_s( 0, slash, &nextTok);
	}

	printf(nextTok);
	fopen_s(&pFile, nextTok, "rb");
	if(pFile == NULL)
	{
		//try just the name
		fopen_s(&pFile, name, "rb");
		if(pFile == NULL)
		{
			//try textFile
			fopen_s(&pFile, textFile, "rb");
			if(pFile == NULL)
			{
				printf("No Valid File\n");
				return ret;
			}		
		}
	}
	
	// Read in header (binary)
	fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);
    
	
	// Get width, height, and depth of texture
	ret.width = tgaHeader.width;
	ret.height = tgaHeader.height;
	sDepth = tgaHeader.bits / 8;
    
	// Put some validity checks here. Very simply, I only understand
	// or care about 8, 24, or 32 bit targa's.
	if(tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
	{
		printf("Invalid File Format\n");
		return ret;
	}
	// Calculate size of image buffer
	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
	ret.size = lImageSize * sizeof(GLbyte);

	// Allocate memory and check for success
	pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
	if(pBits == NULL)
	{
		assert(0);
		return ret;
	}    
	// Read in the bits
	// Check for read error. This should catch RLE or other 
	// weird formats that I don't want to recognize
	if(fread(pBits, lImageSize, 1, pFile) != 1)
		{
		free(pBits);
		printf("Invalid Texture File Format\n");
		return ret;
		}
    
	// Set OpenGL format expected
	switch(sDepth)
		{

		case 3:     // Most likely case
			ret.eFormat = GL_BGR;
			ret.iComponents = GL_RGB;
			break;

		case 4:
			ret.eFormat = GL_BGRA;
			ret.iComponents = GL_RGBA;
			break;
		case 1:
			assert( false);
			// bad case - keenan
			// i commented out
			//  *eFormat = GL_LUMINANCE;
			//  *iComponents = GL_LUMINANCE;
			break;
		default:        // RGB
			// If on the iPhone, TGA's are BGR, and the iPhone does not 
			// support BGR without alpha, but it does support RGB,
			// so a simple swizzle of the red and blue bytes will suffice.
			// For faster iPhone loads however, save your TGA's with an Alpha!

		break;
		}
	
    
    
	// Done with File
	fclose(pFile);
	
	// Return pointer to image data
	ret.bits = pBits;
	return ret;
}

