#include "Common.h"
#include <fstream>
#include <sstream>
#include <iostream>

Object* Loader::read(char file[]) {
	FILE* in = fopen(file, "r");
	char line[256];
	Object* obj = new Object();
	while (fgets(line, sizeof(line), in)) {
		//empty line
		if (strlen(line) < 2)
			continue;
		//vertex
		if (line[0] == 'v' && line[1] == ' ') {
			vertex v;
			int result = sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
			obj->vertexs.push_back(v);
		}
		//face
		if (line[0] == 'f') {
			face f;
			int v, tmp;
			int offset, pos = 2;
			while (sscanf(&line[pos], "%i/%i/%i %n", &v, &tmp, &tmp, &offset) > 0) {
				f.push_back(v);
				pos += offset;
			}
			obj->faces.push_back(f);
		}
		//position
		if (line[0] == '#' && line[1] == 'p') {
			sscanf(line, "#p %f %f %f", &obj->x, &obj->y, &obj->z);
		}
		//rotation
		if (line[0] == '#' && line[1] == 'r') {
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
	//face
	for (int i = 0; i < obj->faces.size(); i++) {
		face& f = obj->faces[i];
		fprintf(out, "f");
		for (int j; j < f.size(); j++)
			fprintf(out, " %i/%i/%i", f[j], 0, 0);
		fprintf(out, "\n");
	}
	//position
	fprintf(out, "#p %f %f %f", obj->x, obj->y, obj->z);
	//rotation
	fprintf(out, "#r %f %f", obj->vertical, obj->horizontal);
	fclose(out);
}	