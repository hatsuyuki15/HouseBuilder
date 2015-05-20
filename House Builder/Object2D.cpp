#include "Common.h"

Object2D::Object2D() {}


Object2D::Object2D(char* texture) {
	this->texture = texture;
	this->textureID = Loader::getInstance()->loadTexture(std::string(texture));
}

void Object2D::setName(char name[]){
	int l = strlen(name);
	this->name = new char[l + 1];
	strncpy(this->name, name, l);
	this->name[l] = 0;
}