#ifndef RIVERSIDE_PIPELINE_CLASS_H
#define RIVERSIDE_PIPELINE_CLASS_H

#include <render/vkCore/rseDevice.hpp>
#include <string>
#include <vector>

namespace RSE {


	struct pipelineCfgInfo {
		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class rsePipeline {
	public:

		rsePipeline(rseDevice& device, const std::string vertPath, const std::string fragPath, const pipelineCfgInfo& cfgInfo);
		~rsePipeline();

		rsePipeline(const rsePipeline&) = delete;
		void operator=(const rsePipeline&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		static pipelineCfgInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

	private:
		static std::vector<char> readFile(const std::string& filePath);

		void createGraphicsPipeline(const std::string vertPath, const std::string fragPath, const pipelineCfgInfo& configInfo);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		rseDevice& mainDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
	};
}

#endif