#include "Common.h"
#include <string>

class Loader {
	static Loader* handle;
	Loader();
public:
	static Loader* getInstance();
	Object* read(std::string file);
	void export(std::string file, std::vector<Instance*>& instances);
};