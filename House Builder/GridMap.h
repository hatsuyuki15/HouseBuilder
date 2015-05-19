#include "Common.h"

const int MAX_GRID_SIZE = 5000;

struct BBox {
	int x, y;
	int width, height;
	bool standby;
};

class GridMap {
	int cellSize;
	Instance* grid[MAX_GRID_SIZE][MAX_GRID_SIZE];
	std::map<Instance*, BBox> bmap;
public:
	Instance* getInstanceAt(int x, int y);
	void toGridLocation(GLfloat worldX, GLfloat worldY, int& x, int& y);
	bool isPuttable(Instance* instance, int x, int y);
	void put(Instance* instance, int x, int y);
	void add(Instance* instance);
	std::vector<Instance*> getInstances();
};

