#include "Common.h"
#include <il/il.h>
#include "TextureManager.h"

TextureManager* TextureManager::instance = NULL;

TextureManager::TextureManager() {	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}

TextureManager::~TextureManager() {

}

TextureManager* TextureManager::getInstance() {
	if (instance != NULL) {
		return instance;
	}
	else {
		instance = new TextureManager();
		return instance;
	}
}

GLuint TextureManager::loadTexture(const char filepath[]){	
	if (textureCache.find(filepath) != textureCache.end()) {
		return textureCache[filepath];
	}
	else{
		GLuint textureID = readTexture(filepath);
		textureCache[filepath] = textureID;
		return textureID;
	}
}

GLuint TextureManager::readTexture(const char filepath[]){
	GLuint imageID;
	// init IL libray
	ilInit();
	// Generate unique image identifier
	ilGenImages(1, &imageID); 
	// Load image
	ilBindImage(imageID);
	ilEnable(IL_ORIGIN_SET);
	//ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	ilLoadImage((ILstring)filepath);
	// convert each pixel to RGBA
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	GLuint textureID;
	// Generate unique texture identifier
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT),
		0,
		GL_RGBA, GL_UNSIGNED_BYTE,
		ilGetData()
	);	
	return textureID;
}