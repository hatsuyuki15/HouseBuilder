#include "Common.h"

class Camera {
	glm::vec3 eye;
	glm::vec3 direction;
	glm::vec3 look;
	glm::vec3 up;

	GLfloat viewAngle;
	GLfloat aspect;
	GLfloat nearPlane;
	GLfloat farPlane;

	static Camera* handle;
	glm::mat4 modelMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	Camera();
	~Camera();
	void loadModelViewProjectionMatrix();
public:
	static Camera* getCamera();
	void move(GLfloat deltaX, GLfloat deltaY, GLfloat deltaZ);
	void zoom(GLfloat ratio);
	void rotate(GLfloat vertical, GLfloat horizontal);
	void resetCamera();
	void setModelMatrix(const glm::mat4 &modelMatrix);
	glm::mat4 getProjectionMatrix();
	glm::mat4 getModelViewMatrix();
	void resetModelMatrix();
};
