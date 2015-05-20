#include "Common.h"
#include <algorithm>

using namespace std;

Object::Object() {
	width = height = -1;
}

GLfloat Object::getWidth() {
	if (width == -1)
		calculateSize();
	return width;
}

GLfloat Object::getHeight() {
	if (height == -1)
		calculateSize();
	return height;
}

void Object::calculateSize() {
	GLfloat minX , minZ, maxX, maxZ;
	minX = maxX = vertexs[0].x;
	minZ = maxZ = vertexs[0].z;
	for (int i = 0; i < vertexs.size(); i++) {
		vertex& v = vertexs[i];
		minX = min(minX, v.x);
		minZ = min(minZ, v.z);
		maxX = max(maxX, v.x);
		maxZ = max(maxZ, v.z);
	}	
	width = maxX - minX;
	height = maxZ - minZ;
}

void Object::setName(char name[]){
	int l = strlen(name);
	this->name = new char[l + 1];
	strncpy(this->name, name, l);
	this->name[l] = 0;
}