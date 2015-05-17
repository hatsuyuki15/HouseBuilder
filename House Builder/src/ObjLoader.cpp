#include <fstream>
#include <sstream>
#include <il/il.h>
#include "ObjLoader.h"

using namespace std;

string ObjLoader::getDirectory(const char filepath[]) {
	string path(filepath);
	string directory;
	int found = path.find_last_of("/\\");
	if (found == string::npos){
		directory = "";
	}
	else {
		directory = path.substr(0, found + 1);
	}
	
	return directory;
}


Obj* ObjLoader::read(char file[]) {
	Obj* obj = new Obj();
	obj->fileName = file;
	obj->resourceFolder = getDirectory(file);
	obj->transform.position = glm::vec3(0, 0, 0);
	obj->transform.scale = glm::vec3(1, 1, 1);
	obj->transform.rotation = glm::vec3(0, 0, 0);
	string err = tinyobj::LoadObj(obj->shapes, obj->materials, obj->fileName, obj->resourceFolder.c_str());
	//log(err);
	return obj;
}


ObjLoader::ObjLoader() {
	ilInit();
}

ObjLoader* ObjLoader::handle;

ObjLoader* ObjLoader::getInstance() {
	if (!handle)
		handle = new ObjLoader();
	return handle;
}
