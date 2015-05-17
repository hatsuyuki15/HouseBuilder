#ifndef _OBJLOADER_H
#define _OBJLOADER_H

#include "Common.h"
#include "Obj.h"

class ObjLoader {
	static ObjLoader* handle;
	ObjLoader();
public:
	static ObjLoader* getInstance();
	Obj* read(char file[]);
	void write(char file[], Obj* obj);
private:
	std::string getDirectory(const char filepath[]);
};


#endif