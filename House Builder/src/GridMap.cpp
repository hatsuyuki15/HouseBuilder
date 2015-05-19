#include "GridMap.h"
#include "Camera.h"

GridMap::GridMap(int cellSize) {
	this->cellSize = cellSize;
	memset(grid, -1, sizeof(grid));
}

void GridMap::addEnvironment(Obj* obj){
	objects.push_back(obj);
}

void GridMap::addObj(Obj* obj, int p, int q) {
	obj->transform.position = glm::vec3(p * cellSize + cellSize / 2.0, 0, q * cellSize + cellSize / 2.0);
	objects.push_back(obj);
	grid[p][q] = objects.size() - 1;
}

void GridMap::remove(Obj* obj) {
	std::vector<Obj*>::iterator it = objects.begin();
	while (it != objects.end())
		if (*it == obj) {
			objects.erase(it);
			break;
		}
}

void GridMap::toGridCoordinate(GLfloat x, GLfloat y, int& p, int& q) {
	p = (int) floor(x / cellSize);
	q = (int) floor(y / cellSize);
}

bool GridMap::isLocationAvailable(int p, int q) {
	return (grid[p][q] == -1);
}

Obj* GridMap::getObject(int p, int q) {
	if (p < 0 || q < 0)
		return NULL;
	if (p >= MAX_GRID_SIZE || q >= MAX_GRID_SIZE)
		return NULL;
	int ord = grid[p][q];
	return (ord >= 0) ? objects[ord] : NULL;	
}

int GridMap::getCellSize() {
	return cellSize;
}

Obj* GridMap::getObjectByMouseXY(int mouseX, int mouseY){
	Camera* camera = Camera::getCamera();
	
	glm::mat4 P = camera->getProjectionMatrix();
	glm::mat4 MV = camera->getModelViewMatrix();

	glm::vec3 winNear = glm::vec3(mouseX, mouseY, 0);
	glm::vec3 winFar = glm::vec3(mouseX, mouseY, 1.0);
	// OpenGL viewport origin is in lower left While window's one is upper left
	glm::vec4 viewport = glm::vec4(0, windowHeight, windowWidth, -windowHeight);

	glm::vec3 oN = glm::unProject(winNear, MV, P, viewport);
	glm::vec3 oF = glm::unProject(winFar, MV, P, viewport);

	glm::vec3 hit;
	glm::vec3 u = oF - oN;	
	u = glm::normalize(u);
	// Now hit = oN + k*u, we have to find k
	float k;
	// if oN is hit point, take a short break
	if (oN.y == 0){
		hit = oN;
	}
	else if (u.y != 0) {
		k = -oN.y / u.y;
		hit = oN + k*u;
	}
	else {
		// something went wrong, the ray is parallel to the map so no hit point
		return NULL;
	}
	// hit is a float point, so we need convert to int, Hmm maybe C++ do automatically for us
	// Dangerous here, round or ceil or floor?
	// use toGridCoordidate func is also a good idea
	
	//int objx = round(hit.x / cellSize);
	//int objy = round(hit.z / cellSize);
	int objx, objy;
	toGridCoordinate(hit.x, hit.z, objx, objy);

	return getObject(objx, objy);
}