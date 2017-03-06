#include "TextureManager.h"

Manager* TextureManager::manager = 0;
TextureManager TextureManager::instance = TextureManager();

TextureManager::TextureManager()
{
	if(manager != 0)
	{
		delete manager;
	}
	this->manager = new Manager();
}

TextureManager::~TextureManager()
{
	delete manager;
	manager = 0;
}

void TextureManager::Activate()
{
	if(instance.manager == 0)
	{
		manager = new Manager();
	}
}

void TextureManager::Deactivate()
{
	delete manager;
	manager = 0;
}


void TextureManager::create(Texture_ID id, const char* fileName, GLenum wrapMode, GLenum minFilter, GLenum maxFilter)
{
	TextureManager::add(new Texture(id, fileName, wrapMode, minFilter, maxFilter), id);
}

void TextureManager::create(Texture_ID id, GLbyte* buffer, unsigned int width, unsigned int height, GLint iComponents, GLenum eFormat, GLenum wrapMode, GLenum minFilter, GLenum maxFilter)
{
	TextureManager::add(new Texture(id, buffer, width, height, iComponents, eFormat, wrapMode, minFilter, maxFilter), id);
}

void TextureManager::add(Texture* inText, Texture_ID id)
{
	instance.manager->add((Node*)inText, id);
}

void TextureManager::remove(Texture_ID id)
{
	instance.manager->remove(id);
}

void TextureManager::clear()
{
	instance.manager->clear();
}

Texture* TextureManager::get(Texture_ID id)
{
	Texture* ret = (Texture*)instance.manager->get(id);
	if (ret == 0)
	{
		return (Texture*)instance.manager->get(DefaultText); 
	}
	else
	{
		return ret;
	}
}