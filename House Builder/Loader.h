#include "Common.h"
#include <string>

class Loader {
	static Loader* handle;
	Loader();
public:
	static Loader* getInstance();
	Object* read(std::string file);
	void write(std::string file, Object* obj);
};