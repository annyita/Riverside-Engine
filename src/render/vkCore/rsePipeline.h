#ifndef RIVERSIDE_PIPELINE_CLASS_H
#define RIVERSIDE_PIPELINE_CLASS_H

#include <render/vkCore/rseDevice.hpp>
#include <string>
#include <vector>

namespace RSE {


	struct pipelineCfgInfo {
		//VkViewport viewport;
		//VkRect2D scissor;
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class rsePipeline {
	public:

		rsePipeline(rseDevice& device, const std::string vertPath, const std::string fragPath, const pipelineCfgInfo& cfgInfo);
		~rsePipeline();

		rsePipeline(const rsePipeline&) = delete;
		rsePipeline operator=(const rsePipeline&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		static void defaultPipelineConfigInfo(pipelineCfgInfo& configInfo);

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