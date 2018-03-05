#ifndef OBJECT_RENDERER_H
#define OBJECT_RENDERER_H

#include <memory>

#include <glm/glm.hpp>

#include "camera.h"
#include "light.h"
#include "material.h"
#include "mesh.h"
#include "shader.h"

class ObjectRenderer{
	public:
		ObjectRenderer();
		void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Camera &camera, const std::vector<std::shared_ptr<Light>> &lights);
		unsigned int getVAO();
		Mesh& getMesh();
		void setMesh(const Mesh &mesh);
		Material& getMaterial();
		Material& getMaterial(unsigned int submeshIndex);
		void setMaterial(const Material &material);
		void setMaterial(unsigned int submeshIndex, const Material &material);
		void setShader(const Shader &shader);
		void reloadMesh();

	private:
		Mesh mesh;
		std::vector<unsigned int> materialIndices;
		std::vector<Material> materials;
		Shader shader;
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;
		void checkErrorAt(const char *location);
};

#endif