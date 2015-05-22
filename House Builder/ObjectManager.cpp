#include "Common.h"
#include <fstream>
#include <string>

using namespace std;

ObjectManager* ObjectManager::handle;

ObjectManager::ObjectManager() {
	Loader* loader = Loader::getInstance();
	string fileName;
	ifstream in("list.db");

	//house
	in >> fileName >> houseTransform.x >> houseTransform.y >> houseTransform.z;
	house = loader->read(fileName);

	//another objects
	while (in.good()) {
		in >> fileName;
		Object* obj = loader->read(fileName);
		objs.push_back(obj);
	}
	in.close();
}

ObjectManager* ObjectManager::getInstance() {
	if (!handle)
		handle = new ObjectManager();
	return handle;
}

Object* ObjectManager::getHouse() {
	return house;
}

glm::vec3 ObjectManager::getHouseTransform() {
	return houseTransform;
}
