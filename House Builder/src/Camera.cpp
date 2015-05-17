#include "Camera.h"

Camera::Camera() {
	eye = glm::vec3(0, 0, 30);
	look = glm::vec3(0, 0, 0);	
	up = glm::vec3(0, 1, 0);

	viewAngle = 45.0;
	aspect = windowWidth / windowHeight;
	nearPlane = 0.1;
	farPlane = 1000;

	glClearColor(1.0, 1.0, 1.0, 0.5);
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(viewAngle, aspect, nearPlane, farPlane);
	//glMatrixMode(GL_MODELVIEW);
	//gluLookAt(eye.x, eye.y, eye.z, look.x, look.y, look.z, up.x, up.y, up.z);
	resetCamera();
}

Camera* Camera::handle;

Camera* Camera::getCamera() {
	if (!handle)
		handle = new Camera();
	return handle;
}

void Camera::resetCamera() {
	//glLoadIdentity();
	modelMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::perspective(viewAngle, aspect, nearPlane, farPlane);
	loadModelViewProjectionMatrix();
}

void Camera::loadModelViewProjectionMatrix() {
	glMatrixMode(GL_PROJECTION);
	viewMatrix = glm::lookAt(eye, look, up);	
	glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;
	glLoadMatrixf((GLfloat*)&MVP[0][0]);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixf(&viewMatrix[0][0]);
	//glMatrixMode(GL_PROJECTION);
}

void Camera::move(GLfloat deltaX, GLfloat deltaY, GLfloat deltaZ) {
	glm::vec3 delta = glm::vec3(deltaX, deltaY, deltaZ);
	eye += delta;
	look += delta;	
	loadModelViewProjectionMatrix();
}

void Camera::zoom(GLfloat ratio) {	
	eye = (eye - look) * ratio + look;
	loadModelViewProjectionMatrix();
}

void Camera::rotate(GLfloat vertical, GLfloat horizontal) {
	modelMatrix *= glm::rotate(vertical, glm::vec3(1, 0, 0));
	modelMatrix *= glm::rotate(horizontal, glm::vec3(0, 1, 0));
	loadModelViewProjectionMatrix();
}

void Camera::setModelMatrix(const glm::mat4 &modelMatrix) {
	this->modelMatrix = modelMatrix;
	loadModelViewProjectionMatrix();
}

void Camera::resetModelMatrix() {
	modelMatrix = glm::mat4(1.0f);
	loadModelViewProjectionMatrix();
}