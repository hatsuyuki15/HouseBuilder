#include "Common.h"

using namespace std;

GridMap::GridMap(GLfloat cellSize, GLfloat originX, GLfloat originY) {
	this->cellSize = cellSize;
	this->originX = originX;
	this->originY = originY;
}

GLfloat GridMap::getWidth() {
	return MAX_GRID_SIZE;
}

GLfloat GridMap::getHeight() {
	return MAX_GRID_SIZE;
}

GLfloat GridMap::getCellSize() {
	return cellSize;
}

Instance* GridMap::getInstanceAt(int x, int y) {
	return grid[x][y];
}

glm::vec2 GridMap::getGridCoordinate(glm::vec3 worldCoordinate) {
	glm::vec2 result;
	result.x = (worldCoordinate.x - originX) / cellSize;
	result.y = (worldCoordinate.z - originY) / cellSize;
	return result;
}

bool GridMap::isPuttable(Instance* instance, int x, int y) {
	BBox& box = bmap[instance];
	for (int i = 0; i < box.width; i++)
		for (int j = 0; j < box.height; j++)
			if (grid[x + i][y + j])
				return false;
	return true;
}

void GridMap::put(Instance* instance, int x, int y) {
	if (!isPuttable(instance, x, y))
		return;
	BBox& box = bmap[instance];
	if (box.standby) {
		box.standby = false;
	} else {
		for (int i = 0; i < box.width; i++)
			for (int j = 0; j < box.height; j++)
				grid[box.x + i][box.y + j] = NULL;
	}
	for (int i = 0; i < box.width; i++)
		for (int j = 0; j < box.height; j++)
			grid[x + i][y + j] = instance;
	box.x = x;
	box.y = y;

	glm::vec3 world = getWorldCoordinate(x, y);
	instance->x = world.x;
	instance->z = world.z;
}

glm::vec3 GridMap::getWorldCoordinate(int x, int y) {
	glm::vec3 result;
	result.x = x * cellSize + originX;
	result.z = y * cellSize + originY;
	result.y = 0;
	return result;
}

void GridMap::add(Instance* instance) {
	BBox* box = new BBox();
	box->standby = true;
	bmap[instance] = *box;
}

vector<Instance*> GridMap::getInstances() {
	vector<Instance*> v;
	map<Instance*, BBox>::iterator it;
	for (it = bmap.begin(); it != bmap.end(); it++)
		v.push_back(it->first);
	return v;
}
