#ifndef RIVERSIDE_APP_CLASS_H
#define RIVERSIDE_APP_CLASS_H

#include <window/window.h>
#include <render/pipeline/rsePipeline.h>
#include <render/device/rseDevice.hpp>

namespace RSE {

	class app {
	public:

		static constexpr int width = 800;
		static constexpr int height = 600;

		void run();

	private:
		Window mainWindow{width, height, "RiversideEngine"};
		rseDevice mainDevice{ mainWindow };
		Pipeline mainPipeline{mainDevice, "Asset/Shader/vertex.spv", "Asset/Shader/fragment.spv", Pipeline::defaultPipelineConfigInfo(width, height) };
	};
}

#endif