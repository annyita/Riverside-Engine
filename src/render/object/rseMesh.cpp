#include "rseMesh.h"

#include <cassert>
#include <cstring>

namespace RSE {
	Mesh::Mesh(rseDevice& device, const Builder& builder) : mainDevice{ device }{
		createVertexBuffers(builder.vertices);
		createIndexBuffers(builder.indices);
	}

	Mesh::~Mesh() {

	}

	void Mesh::draw(VkCommandBuffer commandBuffer){
		if (hasIndexBuffer) {
			vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
		}
		else {
			vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
		}

	}
	
	void Mesh::bind(VkCommandBuffer commandBuffer){
		VkBuffer buffers[] = { vertexBuffer->getBuffer()};
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

		if (hasIndexBuffer) {
			vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
		}
	}

	void Mesh::createVertexBuffers(const std::vector<vertex>& vertices){
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count must be at least 3");
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
		uint32_t vertexSize = sizeof(vertices[0]);

		rseBuffer stagingBuffer{
			mainDevice,
			vertexSize,
			vertexCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		stagingBuffer.map();
		stagingBuffer.writeToBuffer((void*)vertices.data());

		vertexBuffer = std::make_unique<rseBuffer>(
			mainDevice,
			vertexSize,
			vertexCount,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);
	
		mainDevice.copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(), bufferSize);
	}

	void Mesh::createIndexBuffers(const std::vector<uint32_t>& indices){
		indexCount = static_cast<uint32_t>(indices.size());

		hasIndexBuffer = indexCount > 0;

		if (!hasIndexBuffer) return;


		VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
		uint32_t indexSize = sizeof(indices[0]);

		rseBuffer stagingBuffer{
			mainDevice,
			indexSize,
			indexCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		stagingBuffer.map();
		stagingBuffer.writeToBuffer((void*)indices.data());

		indexBuffer = std::make_unique<rseBuffer>(
			mainDevice,
			indexSize,
			indexCount,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		mainDevice.copyBuffer(stagingBuffer.getBuffer(), indexBuffer->getBuffer(), bufferSize);
	}


	// vertices

	std::vector<VkVertexInputBindingDescription> Mesh::vertex::getBindingDescriptions(){
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescriptions;
	}
	std::vector<VkVertexInputAttributeDescription> Mesh::vertex::getAttributeDescriptions(){
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

		attributeDescriptions.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(vertex, position) });
		attributeDescriptions.push_back({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(vertex, normal) });
		attributeDescriptions.push_back({ 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(vertex, colour) });
		attributeDescriptions.push_back({ 3, 0, VK_FORMAT_R32G32_SFLOAT,	offsetof(vertex, uv) });
		attributeDescriptions.push_back({ 4, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(vertex, tangent) });
		attributeDescriptions.push_back({ 5, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(vertex, biTangent) });

		return attributeDescriptions;
	}
}