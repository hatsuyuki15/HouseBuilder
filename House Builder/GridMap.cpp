#include "Common.h"
  
using namespace std;

Instance* MASK = (Instance*)-1;


GridMap::GridMap(GLfloat cellSize) {
	this->cellSize = cellSize;
	originX = -getWidth()  * cellSize / 2;
	originY = -getHeight() * cellSize / 2;
	memset(grid, NULL, sizeof(grid[0][0]) * getWidth() * getHeight());
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

bool GridMap::isInsideGrid(int x, int y) {
	return x >= 0 && x < getWidth() && y >= 0 && y < getHeight();
}

Instance* GridMap::getInstanceAt(int x, int y) {
	if (!isInsideGrid(x, y) || grid[x][y] == MASK)
		return NULL;
	return grid[x][y];
}

glm::vec2 GridMap::getGridCoordinate(glm::vec3 worldCoordinate) {
	glm::vec2 result;
	result.x = (int)((worldCoordinate.x - originX) / cellSize);
	result.y = (int)((worldCoordinate.z - originY) / cellSize);
	return result;
}

bool GridMap::isPuttable(Instance* instance, int x, int y) {
	BBox& box = bmap[instance];
	for (int i = 0; i < box.width; i++)
		for (int j = 0; j < box.height; j++) {
			int p = x + i;
			int q = y + j;
			if (!isInsideGrid(p, q) || (grid[p][q] != instance && grid[p][q]))
				return false;
		}
	return true;
}

bool GridMap::put(Instance* instance, int x, int y, bool ghostMode) {
	bool result = isPuttable(instance, x, y);
	if (!result && !ghostMode)
		return result;

	//clear current occupied cells
	BBox& box = bmap[instance];
	if (!box.standby) {
		for (int i = 0; i < box.width; i++)
			for (int j = 0; j < box.height; j++)
				grid[box.x + i][box.y + j] = NULL;
	}	
	//mark new cells
	if (!ghostMode) {
		for (int i = 0; i < box.width; i++)
			for (int j = 0; j < box.height; j++)
				grid[x + i][y + j] = instance;
	}
	//update instance's position for rendering
	box.x = x;
	box.y = y;
	if (!ghostMode) box.standby = false;
	instance->transform = getWorldCoordinate(x, y);
	return result;
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
	box->width  = ceil(instance->obj->getWidth() / cellSize);
	box->height = ceil(instance->obj->getHeight() / cellSize);
	bmap[instance] = *box;
}

void GridMap::addMask(Instance* instance) {
	const float epsilon = 10;
	vector<vertex>& vertexs = instance->obj->vertexs;
	for (int i = 0; i < vertexs.size(); i++) {
		vertex v = vertexs[i] + instance->transform;
		if (abs(v.y) < epsilon) {
			glm::vec2 p = getGridCoordinate(v);
			if (isInsideGrid(p.x, p.y)) {
				grid[(int)p.x][(int)p.y] = MASK;
			}
		}
	}
	bmap[instance] = *(new BBox());
}

void GridMap::remove(Instance* instance) {
	map<Instance*, BBox>::iterator it;
	it = bmap.find(instance);
	bmap.erase(it);
}

vector<Instance*> GridMap::getInstances() {
	vector<Instance*> v;
	map<Instance*, BBox>::iterator it;
	for (it = bmap.begin(); it != bmap.end(); it++)
		v.push_back(it->first);
	return v;
}
