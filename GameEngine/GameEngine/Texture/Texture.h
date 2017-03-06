#ifndef TEXTURE_H
#define TEXTURE_H

#include "GraphicsEngine.h"
#include "Node.h"

enum Texture_ID
{
	DefaultText,
	BrickText,
	GrassText,
	LatticeText,
	BoxText,
	FrigateText,
	WarbearText = 99, //needed for model importer to work with multiple textures
	WarbearText2 = 100,
	WarbearText3,
	BuggyText,
	MissileText,
	AstroBoyText,
	MayanPyramidText,
	TeddyText,
};


class Texture : public Node
{
public:
	Texture(Texture_ID id, const char* fileName/*, GLenum wrapMode, GLenum minFilter, GLenum maxFilter*/);
	Texture(Texture_ID id, GLbyte* buffer, unsigned int width, unsigned int height, GLint iComponents, GLenum eFormat, GLenum wrapMode, GLenum minFilter, GLenum maxFilter);
	~Texture();

	void setParameters(GLenum wrapMode, GLenum minFilter, GLenum maxFilter);
	void activate();
	void deactivate();
	GLuint getId();
private:
	Texture();

	//Remove
	GLbyte* gltReadTGABits(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat);

protected:
	Texture_ID id;
	ID3D11Device* Device;
	ID3D11DeviceContext* const Context;
	textNum;

};

#endif