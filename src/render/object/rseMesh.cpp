#include "rseMesh.h"

#include <cassert>
#include <cstring>

namespace RSE {
	Mesh::Mesh(rseDevice& device, const std::vector<vertex>& vertices) : mainDevice{ device }{
		createVertexBuffers(vertices);
	}
	Mesh::~Mesh(){
		vkDestroyBuffer(mainDevice.device(), vertexBuffer, nullptr);
		vkFreeMemory(mainDevice.device(), vertexBufferMemory, nullptr);
	}

	void Mesh::draw(VkCommandBuffer commandBuffer){
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}
	
	void Mesh::bind(VkCommandBuffer commandBuffer){
		VkBuffer buffers[] = { vertexBuffer};
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	void Mesh::createVertexBuffers(const std::vector<vertex>& vertices){
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count must be at least 3");
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
		mainDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			vertexBuffer,
			vertexBufferMemory);

		void* data;
		vkMapMemory(mainDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(mainDevice.device(), vertexBufferMemory);
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
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT; // VK_FORMAT_R32G32_SFLOAT
		attributeDescriptions[0].offset = 0;

		//attributeDescriptions[1].binding = 0;
		//attributeDescriptions[1].location = 1;
		//attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		//attributeDescriptions[1].offset = offsetof(vertex, normal);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(vertex, uv);

		return attributeDescriptions;
	}
}