#include "Obj.h"

Obj* Obj::clone() {
	Obj* foo = new Obj();
	foo->fileName = this->fileName;
	foo->shapes = this->shapes;
	foo->materials = this->materials;
	foo->resourceFolder = this->resourceFolder;
	foo->transform = this->transform;
	return foo;
}

glm::mat4 Obj::getModelMatrix() {
	glm::mat4 TranslateMatrix = glm::translate(transform.position);
	glm::mat4 ScaleMatrix = glm::scale(transform.scale);
	glm::mat4 RotationMatrixX = glm::rotate(transform.rotation.x, glm::vec3(1, 0, 0));
	glm::mat4 RotationMatrixY = glm::rotate(transform.rotation.y, glm::vec3(0, 1, 0));
	glm::mat4 RotationMatrixZ = glm::rotate(transform.rotation.z, glm::vec3(0, 0, 1));
	modelMatrix = TranslateMatrix * ScaleMatrix * RotationMatrixX * RotationMatrixY * RotationMatrixZ;
	return modelMatrix;
}