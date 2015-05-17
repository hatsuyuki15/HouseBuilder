#include "Common.h"
#include <fstream>
#include <sstream>
#include <il/il.h>

using namespace std;

string getDirectory(const char filepath[]) {
	string str(filepath);
	string directory = str.substr(0, str.rfind('/') + 1);
	return directory;
}

GLuint loadTexture(const char file[]) {
	unsigned int imageID;
	ilInit();
	ilGenImages(1, &imageID);
	ilBindImage(imageID);
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	ilLoadImage((ILstring)file);
	//TODO: neccesary?
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

void readMaterial(const char file[], Object* obj) {
	FILE* in = fopen(file, "r");
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
		//lighting: diffuse
		if (strcmp(control, "Kd") == 0) {
			sscanf(line, "Kd %f %f %f", &mtl->d1, &mtl->d2, &mtl->d3);
		}
		//lighting: specular
		if (strcmp(control, "Ks") == 0) {
			sscanf(line, "Ks %f %f %f", &mtl->s1, &mtl->s2, &mtl->s3);
		}
		//lighting: ambient
		if (strcmp(control, "Ka") == 0) {
			sscanf(line, "Ka %f %f %f", &mtl->a1, &mtl->a2, &mtl->a3);
		}
		//lighting: image
		if (strcmp(control, "map_Kd") == 0) {
			char imgFile[256];
			sscanf(line, "map_Kd %s", imgFile);
			string str(imgFile);
			string tmp = getDirectory(file) + str;
			mtl->textureID = loadTexture(tmp.c_str());
		}
	}
	fclose(in);
}

Object* Loader::read(char file[]) {
	Object* obj = new Object();
	FILE* in = fopen(file, "r");
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
			string str(materialFile);
			string tmp = getDirectory(file) + str;
			readMaterial(tmp.c_str(), obj);
		}
		//use material
		if (strcmp(control, "usemtl") == 0) {
			char materialName[256];
			sscanf(line, "usemtl %s", materialName);
			string str(materialName);
			mtl = obj->materials[str];
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
			if (mtl)
				f.mtl = mtl;
			obj->faces.push_back(f);
		}
		//position
		if (strcmp(control, "#p")) {
			sscanf(line, "#p %f %f %f", &obj->x, &obj->y, &obj->z);
		}
		//rotation
		if (strcmp(control, "#r")) {
			sscanf(line, "#r %f %f", &obj->vertical, &obj->horizontal);
		}

	}
	fclose(in);
	return obj;
}

void Loader::write(char file[], Object* obj) {
	FILE* out = fopen(file, "w");
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
	//position
	fprintf(out, "#p %f %f %f", obj->x, obj->y, obj->z);
	//rotation
	fprintf(out, "#r %f %f", obj->vertical, obj->horizontal);
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
