#include <assert.h>
#include "Texture.h"
//#include "BitmapFile.h"
#include "Game.h"


#include <stdlib.h>
//#include <sb6.h>
#include "PackageHeaders.h"
#pragma pack(1)
typedef struct
{
    //GLbyte	identsize;              // Size of ID field that follows header (0)
    //GLbyte	colorMapType;           // 0 = None, 1 = paletted
   // GLbyte	imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
    unsigned short	colorMapStart;          // First colour map entry
    unsigned short	colorMapLength;         // Number of colors
    unsigned char 	colorMapBits;   // bits per palette entry
    unsigned short	xstart;                 // image x origin
    unsigned short	ystart;                 // image y origin
    unsigned short	width;                  // width in pixels
    unsigned short	height;                 // height in pixels
    //GLbyte	bits;                   // bits per pixel (8 16, 24, 32)
    //GLbyte	descriptor;             // image descriptor
} TGAHEADER;
#pragma pack(8)




Texture::Texture()
	: Device(GraphicsEngine::getDevice()), Context(GraphicsEngine::getContext())
{
}

Texture::Texture(Texture_ID id, const char* fileName/*, GLenum wrapMode, GLenum minFilter, GLenum maxFilter*/)
	: id(id), Device(GraphicsEngine::getDevice()), Context(GraphicsEngine::getContext())
{
	D3DX11CreateShaderResourceViewFromFile()
	glGenTextures(1, &textNum);
	glActiveTexture(GL_TEXTURE0 + textNum);
	glBindTexture(GL_TEXTURE_2D, textNum);	

	GLint width;
	GLint height;
	GLint icomponents;
	GLenum eFormat;

	GLbyte* bits = gltReadTGABits(fileName, &width, &height, &icomponents, &eFormat);

	setParameters(wrapmode, minFilter, magFilter);

	glTexImage2D(GL_TEXTURE_2D, 0, icomponents, width,height, 0, eFormat, GL_UNSIGNED_BYTE, bits);
	glGenerateMipmap(GL_TEXTURE_2D);

	delete bits;
}

Texture::Texture(Texture_ID id, GLbyte* buffer, unsigned int width, unsigned int height, GLint iComponents, GLenum eFormat, GLenum wrapmode, GLenum minFilter, GLenum magFilter)
	: id(id)
{
	glGenTextures(1, &textNum);
	glActiveTexture(GL_TEXTURE0 + textNum);
	glBindTexture(GL_TEXTURE_2D, textNum);	
	
	setParameters(wrapmode, minFilter, magFilter);
	//buffer, width, height, iComponents; eFormat; wrapmode; minFilter; magFilter;
	glTexImage2D(GL_TEXTURE_2D, 0, iComponents, width, height, 0, eFormat, GL_UNSIGNED_BYTE, buffer);
	
	if(minFilter == GL_LINEAR_MIPMAP_LINEAR  || minFilter == GL_LINEAR_MIPMAP_NEAREST ||
	   minFilter == GL_NEAREST_MIPMAP_LINEAR || minFilter == GL_NEAREST_MIPMAP_NEAREST)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

GLbyte* Texture::gltReadTGABits(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
	{
    FILE *pFile;			// File pointer
    TGAHEADER tgaHeader;		// TGA file header
    unsigned long lImageSize;		// Size in bytes of image
    short sDepth;			// Pixel depth;
    GLbyte	*pBits = NULL;          // Pointer to bits
    
    // Default/Failed values
    *iWidth = 0;
    *iHeight = 0;
    *eFormat = GL_RGB;
    *iComponents = GL_RGB;
    
    // Attempt to open the file
	fopen_s(&pFile, szFileName, "rb");
    if(pFile == NULL)
        return NULL;
	
    // Read in header (binary)
    fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, pFile);
    
	
    // Get width, height, and depth of texture
    *iWidth = tgaHeader.width;
    *iHeight = tgaHeader.height;
    sDepth = tgaHeader.bits / 8;
    
    // Put some validity checks here. Very simply, I only understand
    // or care about 8, 24, or 32 bit targa's.
    if(tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
        return NULL;
	
    // Calculate size of image buffer
    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
    
    // Allocate memory and check for success
    pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
    if(pBits == NULL)
        return NULL;
    
    // Read in the bits
    // Check for read error. This should catch RLE or other 
    // weird formats that I don't want to recognize
    if(fread(pBits, lImageSize, 1, pFile) != 1)
		{
        free(pBits);
        return NULL;
		}
    
    // Set OpenGL format expected
    switch(sDepth)
		{

        case 3:     // Most likely case
            *eFormat = GL_BGR;
            *iComponents = GL_RGB;
            break;

        case 4:
            *eFormat = GL_BGRA;
            *iComponents = GL_RGBA;
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
    return pBits;
	}

Texture::~Texture()
{
	glDeleteTextures(1, &textNum);
}

void Texture::setParameters(GLenum wrapMode, GLenum minFilter, GLenum magFilter)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void Texture::activate()
{
	if (id != 0)
	{
		glActiveTexture(GL_TEXTURE0 + textNum);
		glBindTexture(GL_TEXTURE_2D, textNum);

	}
	else
	{
		assert(0); //Texture not loaded
	}
}

void Texture::deactivate()
{
	glActiveTexture(GL_TEXTURE0 + textNum);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getId()
{
	return textNum;
}



