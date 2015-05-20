#include "Common.h"

class Loader {
	static Loader* handle;
	Loader();
public:
	static Loader* getInstance();
	Object* read(std::string file);
	void export(std::string file, std::vector<Instance*>& instances);
	GLuint loadTexture(std::string file);
	void Loader::readMaterial(std::string file, Object* obj);
};