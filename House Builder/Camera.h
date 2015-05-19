#include "Common.h"

class Camera {
	glm::vec3 eye, center;
	GLfloat vertical, horizontal;
	GLfloat viewAngle, aspect, zNear, zFar;
	static Camera* handle;
	Camera();
	void update();
public:
	static Camera* getCamera();
	void move(GLfloat deltaX, GLfloat deltaY, GLfloat deltaZ);
	void zoom(GLfloat ratio);
	void rotate(GLfloat dVertical, GLfloat dHorizontal);
};

