#include "rseModel.h"
#include <util/UUID.h>

#include <assimp/pbrmaterial.h>
#include <assimp/material.h>

#include <iostream>

namespace RSE {
	
	Model::Model(const char* file, rseDevice& device) : mainDevice{ device } {
		this->uuid = UUID::returnHandle();
		path = file;
		loadModel();
	}

	Model::~Model(){
		for (size_t i = 0; i < localTransformations.size(); i++) {
			localTransformations.erase(localTransformations.begin() + i);
		}
		localTransformations.clear();
	}

	void Model::loadModel(){
		Assimp::Importer import;

		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene){
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			totalVertices += mesh->mNumVertices;
			totalIndices += mesh->mNumFaces * 3;
			totalBones += mesh->mNumBones;

			processPositions(node);
			meshes.push_back(processMesh(mesh, scene));
		}

		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene);
		}
	}

	void Model::processPositions(aiNode* node){
		aiMatrix4x4 localTransform = node->mTransformation;
		aiVector3D position; aiVector3D scale; aiQuaternion rotation;
		localTransform.Decompose(scale, rotation, position);

		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::translate(matrix, glm::vec3(position.x, position.y, position.z));
		glm::mat4 rotation_matrix = glm::mat4_cast(glm::quat(rotation.w, rotation.x, rotation.y, rotation.z));
		matrix = matrix * rotation_matrix;
		matrix = glm::scale(matrix, glm::vec3(scale.x, scale.y, scale.z));

		localTransformation newLocalTransformation;
		newLocalTransformation.localPosition = glm::vec3(position.x, position.y, position.z);
		newLocalTransformation.localScale = glm::vec3(scale.x, scale.y, scale.z);
		newLocalTransformation.localqRotation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
		glm::vec3 euler_radians = glm::eulerAngles(newLocalTransformation.localqRotation);
		newLocalTransformation.localRotation = glm::degrees(euler_radians);

		newLocalTransformation.matrix = matrix;

		localTransformations.push_back(newLocalTransformation);
	}

	std::vector<Mesh::vertex> Model::assembleVertices(aiMesh* mesh){

		std::vector<Mesh::vertex> vertices;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			Mesh::vertex vertex;
			//SetVertexBoneDataToDefault(vertex); // feed dummy data

			// process vertex positions, normals and texture coordinates
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
			
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.tangent = vector;

			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.biTangent = vector;

			vertex.colour = glm::vec3(1.0f, 1.0f, 1.0f);

			if (mesh->mTextureCoords[0]){ // does the mesh contain texture coordinates? // determine colour here
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.uv = vec;
			}
			else
				vertex.uv = glm::vec2(0.0f, 0.0f);


			vertices.push_back(vertex);
		}

		return vertices;

		//return std::vector<Mesh::vertex>();
	}

	std::vector<uint32_t> Model::assembleIndices(aiMesh* mesh){
		std::vector<uint32_t> indices;

		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		return indices;
	}

	std::shared_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene){

		Mesh::Builder newBuilder;

		newBuilder.vertices = assembleVertices(mesh);
		newBuilder.indices = assembleIndices(mesh);

		auto newMesh = std::make_shared<Mesh>(mainDevice, newBuilder);
		newMesh->name = mesh->mName.C_Str();

		return newMesh;
	}


}