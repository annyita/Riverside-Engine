#ifndef RIVERSIDE_APP_CLASS_H
#define RIVERSIDE_APP_CLASS_H

#include <window/window.h>
#include <render/vkCore/rseDevice.hpp>
#include <render/rseRenderer.h>

#include <game/scene/entityHandler.h>

#include <memory>
#include <vector>

namespace RSE {

	class app {
	public:

		static constexpr int width = 1920;
		static constexpr int height = 1080;

		app();
		~app();

		app(const app&) = delete;
		app& operator=(const app&) = delete;

		void run();

	private:


		void loadEntities();

		Window mainWindow{width, height, "Riverside Engine"};
		rseDevice mainDevice{ mainWindow };
		rseRenderer mainRenderer{ mainWindow, mainDevice };
	};
}

#endif