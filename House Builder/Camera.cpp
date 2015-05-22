#include "Common.h"

Camera::Camera() {
	eye = glm::vec3(0, 300, 700);
	look = glm::vec3(0, 0, 0);
	up = glm::vec3(0, 7, -3);

	viewAngle = 45.0;
	aspect = 4.0f / 3.0f;
	nearPlane = 0.1;
	farPlane = 2000;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(viewAngle, aspect, nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
	resetCamera();
	rotate(0, -0.5);
}

Camera* Camera::handle;

Camera* Camera::getCamera() {
	if (!handle)
		handle = new Camera();
	return handle;
}

void Camera::resetCamera() {
	modelMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::perspective(viewAngle, aspect, nearPlane, farPlane);
	loadModelViewProjectionMatrix();

}

void Camera::loadModelViewProjectionMatrix() {
	glMatrixMode(GL_PROJECTION);
	viewMatrix = glm::lookAt(eye, look, up);
	glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;
	glLoadMatrixf((GLfloat*)&MVP[0][0]);
}

void Camera::move(GLfloat deltaX, GLfloat deltaY, GLfloat deltaZ) {
	glm::vec3 delta = glm::vec3(deltaX, deltaY, deltaZ);
	eye += delta;
	look += delta;
	loadModelViewProjectionMatrix();
}

void Camera::zoom(GLfloat ratio) {
	eye = (eye - look) / ratio + look;
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

glm::mat4 Camera::getProjectionMatrix() {
	return projectionMatrix;
}

glm::mat4 Camera::getModelViewMatrix() {
	viewMatrix = glm::lookAt(eye, look, up);
	glm::mat4 MV = viewMatrix * modelMatrix;
	return MV;
}