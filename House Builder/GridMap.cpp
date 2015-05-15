#include "Common.h"

GridMap::GridMap(int cellSize) {
	this->cellSize = cellSize;
	memset(grid, -1, sizeof(grid));
}

void GridMap::add(Object* obj) {
	objects.push_back(obj);
}

void GridMap::remove(Object* obj) {
	vector<Object*>::iterator it = objects.begin();
	while (it != objects.end())
		if (*it == obj) {
			objects.erase(it);
			break;
		}
}

void GridMap::toGridCoordinate(GLfloat x, GLfloat y, int& p, int& q) {
	p = x / cellSize;
	q = y / cellSize;
}

bool GridMap::isLocationAvailable(int p, int q) {
	return (grid[p][q] == -1);
}

Object* GridMap::getObject(int p, int q) {
	int ord = grid[p][q];
	return (ord >= 0) ? objects[ord] : NULL;
}