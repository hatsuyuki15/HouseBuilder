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

GLuint Loader::loadTexture(string file) {
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
	return textureID;
}

void Loader::readMaterial(string file, Object* obj) {
	FILE* in = fopen(file.c_str(), "r");
	char line[256];
	material* mtl = new material();
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
			sscanf(line, "Ka %f %f %f", &mtl->ambient[0], &mtl->ambient[1], &mtl->ambient[2]);
		}
		//lighting: specular
		if (strcmp(control, "Ks") == 0) {
			sscanf(line, "Ks %f %f %f", &mtl->specular[0], &mtl->specular[1], &mtl->specular[2]);
		}
		//lighting: diffuse
		if (strcmp(control, "Kd") == 0) {
			sscanf(line, "Kd %f %f %f", &mtl->diffuse[0], &mtl->diffuse[1], &mtl->diffuse[2]);
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
	material* mtl = NULL;
	FILE* in = fopen(file.c_str(), "r");
	char line[256];
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
			int offset, pos = 2;
			char set[256];
			while (sscanf(&line[pos], "%s %n", set, &offset) > 0) {
				fvertex v;
				v.iv = v.ivn = v.ivt = 0;
				sscanf(set, "%i/%i/%i", &v.iv, &v.ivt, &v.ivn);
				sscanf(set, "%i//%i", &v.iv, &v.ivn);
				f.fv.push_back(v);
				pos += offset;
			}
			f.mtl = mtl;
			obj->faces.push_back(f);
		}
	}
	fclose(in);
	return obj;
}

void Loader::export(string file, vector<Instance*>& instances) {
	FILE* out = fopen(file.c_str(), "w");
	int numV, numVt, numVn;
	numV = numVt = numVn = 0;
	for (int i = 0; i < instances.size(); i++) {
		Instance* ins = instances[i];
		Object* obj = ins->obj;
		//vertex
		for (int i = 0; i < obj->vertexs.size(); i++) {
			glm::vec3 v = obj->vertexs[i] + ins->transform;
			fprintf(out, "v %f %f %f\n", v.x, v.y, v.z);
		}
		//normal
		for (int i = 0; i < obj->nvertexs.size(); i++) {
			vertex& v = obj->nvertexs[i];
			fprintf(out, "vn %f %f %f\n", v.x, v.y, v.z);
		}
		//texture
		for (int i = 0; i < obj->tvertexs.size(); i++) {
			vertex& v = obj->tvertexs[i];
			fprintf(out, "vt %f %f %f\n", v.x, v.y, v.z);
		}
		//face
		for (int i = 0; i < obj->faces.size(); i++) {
			face& f = obj->faces[i];
			fprintf(out, "f");
			for (int j = 0; j < f.fv.size(); j++) {
				fvertex& fv = f.fv[j];
				if (fv.iv) fprintf(out, " %i", fv.iv + numV);
				if (fv.ivt) fprintf(out, "/%i", fv.ivt + numVt);
				if (fv.ivn) {
					if (!fv.ivt)
						fprintf(out, "//%i", fv.ivn + numVn);
					else
						fprintf(out, "/%i", fv.ivn + numVn);
				}
			}
			fprintf(out, "\n");
		}
		numV += obj->vertexs.size();
		numVt += obj->tvertexs.size();
		numVn += obj->nvertexs.size();
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
