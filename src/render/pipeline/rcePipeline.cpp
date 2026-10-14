#include "rcePipeline.h"

#include <fstream>
#include <stdexcept>
#include <iostream>

RSE::Pipeline::Pipeline(const std::string vertPath, const std::string fragPath){
	createGraphicsPipeline(vertPath, fragPath);
}

std::vector<char> RSE::Pipeline::readFile(const std::string& filePath){

	std::ifstream file(filePath, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file: " + filePath);
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

void RSE::Pipeline::createGraphicsPipeline(const std::string vertPath, const std::string fragPath){

	auto vertCode = readFile(vertPath);
	auto fragCode = readFile(fragPath);

	std::cout << "Vertex shader code size: " << vertCode.size() << " bytes" << std::endl;
	std::cout << "Fragment shader code size: " << fragCode.size() << " bytes" << std::endl;

}
