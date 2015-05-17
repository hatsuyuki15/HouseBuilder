#include <stdio.h>
#include <stdlib.h>
#include "TextureManager.h"
#include "Render.h"

Render::Render() {
	glEnable(GL_DEPTH_TEST);	
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_NORMALIZE);
	// Somewhere in the initialization part of your program…
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Create light components
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
	GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat position[] = { -1.5f, 1.0f, -4.0f, 1.0f };

	// Assign created components to GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	camera = Camera::getCamera();
}

void Render::setGridMap(GridMap* map) {
	this->map = map;
}

void Render::renderObj(Obj* obj) {
	if (obj == NULL){
		//log("Obj is NULL");
		return;
	}
	glPushMatrix();
	
	// Rotate
	glRotatef(obj->transform.rotation.x, 1, 0, 0);
	glRotatef(obj->transform.rotation.y, 0, 1, 0);
	glRotatef(obj->transform.rotation.z, 0, 0, 1);
	// Scale
	glScalef(obj->transform.scale.x, obj->transform.scale.y, obj->transform.scale.z);
	// Translate
	glTranslatef(obj->transform.position.x, obj->transform.position.y, obj->transform.position.z);
	//glutWireSphere(10.f, 16, 16);
	Shapes shapes = obj->shapes;	
	for (size_t i = 0; i < shapes.size(); i++) {		
		tinyobj::mesh_t mesh = shapes[i].mesh;
		//glBegin(GL_TRIANGLES);
		if (mesh.material_ids.size() > 0 || mesh.material_ids[0] != -1) {
			int material_id = mesh.material_ids[0];			
			tinyobj::material_t material = obj->materials[material_id];
			std::string texturePath = obj->resourceFolder + material.diffuse_texname;
			GLuint textureID = TextureManager::getInstance()->loadTexture(texturePath.c_str());
			glBindTexture(GL_TEXTURE_2D, textureID);
		}
		int face_nums = mesh.indices.size() / 3;
		for (size_t f = 0; f < face_nums; ++f) {
			glBegin(GL_TRIANGLE_FAN);
			for (int j = 0; j < 3; ++j) {
				int idm = mesh.indices[f*3 + j]; // material index
				int idv = idm * 3; // vertex index
				int idt = idm * 2; // texture coord
				glNormal3f(mesh.normals[idv], mesh.normals[idv + 1], mesh.normals[idv + 2]);
				glTexCoord2f(mesh.texcoords[idt], mesh.texcoords[idt + 1]);
				glVertex3f(mesh.positions[idv], mesh.positions[idv + 1], mesh.positions[idv + 2]);								
			}
			glEnd();
		}		
	}
	glPopMatrix();
}

void Render::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
	//glutWireSphere(10.f, 16, 16);
	for (int i = 0; i < map->objects.size(); i++) {
		renderObj(map->objects[i]);
	}	
	glFlush();
}