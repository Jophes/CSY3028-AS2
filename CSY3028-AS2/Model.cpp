#include "Model.h"
#include "Mesh.h"
#include "MeshRenderer.h"

Model::Model() {
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}

Model::Model(Camera* camera, Mesh* msh, Material* mat) {
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	
	cam = camera;
	mesh = msh;
	material = mat;
}

void Model::Draw() {
	glm::mat4 modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, scale);

	modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));

	glm::mat4 mv = cam->view * modelMatrix;
	glm::mat4 mvp = cam->proj * mv;

	// USE MATERIAL DATA

	GLuint location = glGetUniformLocation(material->shaderProg->programme, "MV");
	glUniformMatrix4fv(location, 1, GL_FALSE, &mv[0][0]);
	location = glGetUniformLocation(material->shaderProg->programme, "MVP");
	glUniformMatrix4fv(location, 1, GL_FALSE, &mvp[0][0]);

	glBindVertexArray(mesh->vertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, mesh->vertexCount);
}