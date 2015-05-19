#ifndef _GRID_MAP_H
#define _GRID_MAP_H

#include "Common.h"
#include "Obj.h"
#include <vector>

const int MAX_GRID_SIZE = 5000;

class GridMap {
	int cellSize;
	int grid[MAX_GRID_SIZE][MAX_GRID_SIZE];
public:	
	GridMap(int cellSize);
	void addEnvironment(Obj* obj);
	void addObj(Obj* obj, int p, int q);
	void remove(Obj* obj);
	int getCellSize();
	void toGridCoordinate(GLfloat x, GLfloat y, int& p, int& q);
	bool isLocationAvailable(int p, int q);
	Obj* getObject(int p, int q);
	Obj* getObjectByMouseXY(int mouseX, int mouseY);
	void setObject(Obj* obj, int p, int q);
	std::vector<Obj*> objects;
};

#endif