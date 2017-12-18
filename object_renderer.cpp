#include "object_renderer.hpp"

void ObjectRenderer::init(){
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mesh.ATTRIBUTE_SIZE * sizeof(float), (void *)0);                     // Vertex
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, mesh.ATTRIBUTE_SIZE * sizeof(float), (void *)(3 * sizeof(float)));   // Normal
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, mesh.ATTRIBUTE_SIZE * sizeof(float), (void *)(6 * sizeof(float)));   // Color
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, mesh.ATTRIBUTE_SIZE * sizeof(float), (void *)(9 * sizeof(float)));   // Texture
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
	glBindVertexArray(0);

	checkErrorAt("Object Renderer Initialization");
}

/**
 * Draws this object. Assumes it is only called within some sort of display function.
 * 
 * @param model the model matrix at the time of drawing the object.
 * @param view the view matrix at the time of drawing the object.
 * @param projection the projection matrix at the time of drawing the object.
 * @param camera the Camera the object is being viewed by.
 */
void ObjectRenderer::draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Camera &camera){
	Light light;
	light.pos = glm::vec3(0, 2, 0);
	light.ambient = glm::vec3(0.5f, 0.5f, 0.5f);
	light.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	light.specular = glm::vec3(0.7f, 0.5f, 0.5f);

	glm::vec3 cameraPos(camera.getPosition());

	shader.use();

	glm::mat3 normalModel(glm::transpose(glm::inverse(model)));
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix3fv(glGetUniformLocation(shader.getID(), "normalModel"), 1, GL_FALSE, &normalModel[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "projection"), 1, GL_FALSE, &projection[0][0]);

	glm::mat4 coordinateTransform = projection * view * model;
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "coordinateTransform"), 1, GL_FALSE, &coordinateTransform[0][0]);

	glUniform1i(glGetUniformLocation(shader.getID(), "texture1"), 0);

	glUniform3fv(glGetUniformLocation(shader.getID(), "material.ambient"), 1, &material.ambient[0]);
	glUniform3fv(glGetUniformLocation(shader.getID(), "material.diffuse"), 1, &material.diffuse[0]);
	glUniform3fv(glGetUniformLocation(shader.getID(), "material.specular"), 1, &material.specular[0]);
	glUniform1f(glGetUniformLocation(shader.getID(), "material.shininess"), material.shininess);

	glUniform3fv(glGetUniformLocation(shader.getID(), "light.pos"), 1, &light.pos[0]);
	glUniform3fv(glGetUniformLocation(shader.getID(), "light.ambient"), 1, &light.ambient[0]);
	glUniform3fv(glGetUniformLocation(shader.getID(), "light.diffuse"), 1, &light.diffuse[0]);
	glUniform3fv(glGetUniformLocation(shader.getID(), "light.specular"), 1, &light.specular[0]);

	glUniform3fv(glGetUniformLocation(shader.getID(), "cameraPos"), 1, &cameraPos[0]);

	glBindTexture(GL_TEXTURE_2D, texture.getID());

	glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, mesh.getNumTriangles() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	checkErrorAt("ObjectRenderer draw");
}

/**
 * Copies the current mesh into the buffer. If this object's currently set mesh object is changed after it is set,
 * this function must be called before changes take effect. (Note: Using setMesh to set the mesh to a new Mesh object
 * calls this method by default.)
 */
void ObjectRenderer::reloadMesh(){
	glBindVertexArray(VAO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.getNumVertices() * mesh.ATTRIBUTE_SIZE, mesh.getVertexData(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.getNumTriangles() * 3, mesh.getIndexData(), GL_STATIC_DRAW);
	glBindVertexArray(0);
}

unsigned int ObjectRenderer::getVAO(){
	return this->VAO;
}

/**
 * Sets the mesh of the object renderer and loads the data into the buffers.
 */
void ObjectRenderer::setMesh(const Mesh &mesh){
	this->mesh = mesh;
	reloadMesh();
}

void ObjectRenderer::setMaterial(const Material &material){
	this->material = material;
}

void ObjectRenderer::setTexture(const Texture &texture){
	this->texture = texture;
}

void ObjectRenderer::setShader(const ShaderProgram &shader){
	this->shader = shader;
}

void ObjectRenderer::checkErrorAt(const char *location){
	GLenum err;
	if((err = glGetError()) != GL_NO_ERROR){
		std::cout << "Error at " << location << ": " << err << std::endl;
	}
}