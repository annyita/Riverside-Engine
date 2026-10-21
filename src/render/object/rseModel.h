#pragma once

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>

#include <render/object/rseMesh.h>

#include <memory>

namespace RSE {

	class Model
	{
	public:

		Model(const char* file, rseDevice& device);
		~Model();

		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

		uint64_t uuid;

		std::vector<std::shared_ptr<Mesh>> meshes{};

		struct localTransformation {
			glm::vec3 localPosition = glm::vec3(0.0f);
			glm::quat localqRotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
			glm::vec3 localRotation = glm::vec3(0.0f);
			glm::vec3 localScale = glm::vec3(0.0f);
			glm::mat4 matrix = glm::mat4(1.0f);
		};

		std::vector<localTransformation> localTransformations;

	private:

		std::string path;
		std::string directory;

		void loadModel();
		void processNode(aiNode* node, const aiScene* scene);
		void processPositions(aiNode* node);


		std::vector<Mesh::vertex> assembleVertices(aiMesh* mesh);
		std::vector<uint32_t> assembleIndices(aiMesh* mesh);

		std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);

		int totalVertices = 0;
		int totalIndices = 0;
		int totalBones = 0;

		rseDevice& mainDevice;
	};

}