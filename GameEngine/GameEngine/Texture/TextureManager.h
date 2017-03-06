#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "Manager.h"
#include "Texture.h"

class TextureManager
{
public:
	
	static void Activate();
	static void create(Texture_ID id, const char* fileName, GLenum wrapMode, GLenum minFilter, GLenum maxFilter);
	static void create(Texture_ID id, GLbyte* buffer, unsigned int width, unsigned int height, GLint iComponents, GLenum eFormat, GLenum wrapMode, GLenum minFilter, GLenum maxFilter);
	static void add(Texture* inText, Texture_ID id);
	static void remove(Texture_ID id);
	static void clear();
	static Texture* get(Texture_ID id);
	static void Deactivate();
private:
	TextureManager();
	~TextureManager();
	
	static TextureManager instance;
	static Manager*		  manager;

};

#endif // ! TEXTUREMANAGER_H
