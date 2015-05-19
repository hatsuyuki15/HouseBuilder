#include "Common.h"
#include <fstream>
#include <sstream>
#include <il/il.h>
#include <iostream>

using namespace std;

string getAbsolutePath(string siblingFile, char fileName[]) {
	size_t pos = siblingFile.rfind('\\');
	string directory = siblingFile.substr(0, pos + 1);
	return directory + string(fileName);
}

GLuint loadTexture(string file) {
	unsigned int imageID;
	ilInit();
	ilGenImages(1, &imageID);
	ilBindImage(imageID);
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	ilLoadImage((ILstring)file.c_str());
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT),
		0, GL_RGBA, GL_UNSIGNED_BYTE,
		ilGetData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	return imageID;
}

void readMaterial(string file, Object* obj) {
	FILE* in = fopen(file.c_str(), "r");
	char line[256];
	material* mtl = new material;
	while (fgets(line, sizeof(line), in)) {
		char control[256];
		//empty or invalid line
		if (sscanf(line, "%s", control) <= 0)
			continue;
		//new material
		if (strcmp(control, "newmtl") == 0)	{
			char mtlName[256];
			sscanf(line, "newmtl %s", mtlName);
			string str(mtlName);
			mtl = new material();
			mtl->name = str;
			obj->materials[str] = mtl;
		}
		//lighting: ambient
		if (strcmp(control, "Ka") == 0) {
			sscanf(line, "Kd %f %f %f", &mtl->ambient.x, &mtl->ambient.y, &mtl->ambient.z);
		}
		//lighting: specular
		if (strcmp(control, "Ks") == 0) {
			sscanf(line, "Ks %f %f %f", &mtl->specular.x, &mtl->specular.y, &mtl->specular.z);
		}
		//lighting: diffuse
		if (strcmp(control, "Kd") == 0) {
			sscanf(line, "Ka %f %f %f", &mtl->diffuse.x, &mtl->diffuse.y, &mtl->diffuse.z);
		}
		//lighting: image
		if (strcmp(control, "map_Kd") == 0) {
			char imgFile[256];
			sscanf(line, "map_Kd %s", imgFile);
			mtl->textureID = loadTexture(getAbsolutePath(file, imgFile));
		}
	}
	fclose(in);
}

Object* Loader::read(string file) {
	Object* obj = new Object();
	FILE* in = fopen(file.c_str(), "r");
	char line[256];
	material* mtl = NULL;
	while (fgets(line, sizeof(line), in)) {
		char control[256];
		//empty or invalid line
		if (sscanf(line, "%s", control) <= 0)
			continue;
		//load material file
		if (strcmp(control, "mtllib") == 0) {
			char materialFile[256];
			sscanf(line, "mtllib %s", materialFile);
			readMaterial(getAbsolutePath(file, materialFile), obj);
		}
		//use material
		if (strcmp(control, "usemtl") == 0) {
			char materialName[256];
			sscanf(line, "usemtl %s", materialName);
			mtl = obj->materials[string(materialName)];
		}
		//vertex
		if (strcmp(control, "v") == 0) {
			vertex v;
			sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
			obj->vertexs.push_back(v);
		}
		//normal vertex
		if (strcmp(control, "vn") == 0) {
			vertex v;
			sscanf(line, "vn %f %f %f", &v.x, &v.y, &v.z);
			obj->nvertexs.push_back(v);
		}
		//texture vertex
		if (strcmp(control, "vt") == 0) {
			vertex v;
			sscanf(line, "vt %f %f", &v.x, &v.y);
			obj->tvertexs.push_back(v);
		}
		//face
		if (strcmp(control, "f") == 0) {
			face f;
			fvertex fv;
			int offset, pos = 2;
			while (sscanf(&line[pos], "%i/%i/%i %n", &fv.iv, &fv.ivt, &fv.ivn, &offset) > 0) {
				f.fv.push_back(fv);
				pos += offset;
			}
			f.mtl = mtl;
			obj->faces.push_back(f);
		}
	}
	fclose(in);
	return obj;
}

void Loader::write(string file, Object* obj) {
	FILE* out = fopen(file.c_str(), "w");
	//vertex
	for (int i = 0; i < obj->vertexs.size(); i++) {
		vertex& v = obj->vertexs[i];
		fprintf(out, "v %f %f %f\n", v.x, v.y, v.z);
	}
	//normal vertex
	for (int i = 0; i < obj->nvertexs.size(); i++) {
		vertex& v = obj->nvertexs[i];
		fprintf(out, "vn %f %f %f\n", v.x, v.y, v.z);
	}
	//texture vertex
	for (int i = 0; i < obj->tvertexs.size(); i++) {
		vertex& v = obj->tvertexs[i];
		fprintf(out, "vt %f %f\n", v.x, v.y);
	}
	//face
	material* lastMtl = NULL;
	for (int i = 0; i < obj->faces.size(); i++) {
		face& f = obj->faces[i];
		if (f.mtl != lastMtl) {
			fprintf(out, "usemtl %s\n", f.mtl->name);
			lastMtl = f.mtl;
		}
		fprintf(out, "f");
		for (int j; j < f.fv.size(); j++) {
			fvertex& v = f.fv[j];
			fprintf(out, " %i/%i/%i", v.iv, v.ivn, v.ivt);
		}
		fprintf(out, "\n");
	}
	fclose(out);
}	

Loader::Loader() {
	ilInit();
}

Loader* Loader::handle;

Loader* Loader::getInstance() {
	if (!handle)
		handle = new Loader();
	return handle;
}
