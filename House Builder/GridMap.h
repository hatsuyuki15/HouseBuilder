#include "Common.h"

const int MAX_GRID_SIZE = 5000;

class GridMap {
	int cellSize;
	int grid[MAX_GRID_SIZE][MAX_GRID_SIZE];
public:
	vector<Object*> objects;
	GridMap(int cellSize);
	void add(Object *obj);
	void remove(Object *obj);
	void toGridCoordinate(GLfloat x, GLfloat y, int& p, int& q);
	bool isLocationAvailable(int p, int q);
	Object* getObject(int p, int q);
	void setObject(Object* obj, int p, int q);
};

