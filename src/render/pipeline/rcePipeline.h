#ifndef RIVERSIDE_PIPELINE_CLASS_H
#define RIVERSIDE_PIPELINE_CLASS_H

#include <string>
#include <vector>

namespace RSE {

	class Pipeline {
	public:

		Pipeline(const std::string vertPath, const std::string fragPath);

	private:
		static std::vector<char> readFile(const std::string& filePath);

		void createGraphicsPipeline(const std::string vertPath, const std::string fragPath);

	};
}

#endif