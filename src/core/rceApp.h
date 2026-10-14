#ifndef RIVERSIDE_APP_CLASS_H
#define RIVERSIDE_APP_CLASS_H

#include <window/window.h>
#include <render/pipeline/rcePipeline.h>

namespace RSE {

	class app {
	public:

		static constexpr int width = 800;
		static constexpr int height = 600;

		void run();

	private:
		RSE::Window mainWindow{width, height, "RiversideEngine"};
		RSE::Pipeline mainPipeline{ "Asset/Shader/vertex.spv", "Asset/Shader/fragment.spv" };
	};
}

#endif