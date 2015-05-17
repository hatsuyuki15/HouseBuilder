#ifndef _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H

#include "Common.h"
#include <map>

class TextureManager {
private:
	static TextureManager* instance;
	std::map<std::string, GLuint> textureCache;
	TextureManager();
	GLuint readTexture(const char filepath[]);
public:	
	static TextureManager* getInstance();
	GLuint loadTexture(const char filepath[]);
	~TextureManager();
};

#endif