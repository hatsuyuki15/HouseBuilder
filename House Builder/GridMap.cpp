#include "Common.h"

using namespace std;

Instance* GridMap::getInstanceAt(int x, int y) {
	return grid[x][y];
}

void GridMap::toGridLocation(GLfloat worldX, GLfloat worldY, int& x, int& y) {
	x = worldX / cellSize;
	y = worldY / cellSize;
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
