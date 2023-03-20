#pragma once

#include "Windows.hpp"
#include "Device.hpp"
#include "GameObject.hpp"
#include "Renderer.hpp"

#include <memory>
#include <vector>

namespace Engine
{

	class Application
	{
	public:
		Application();
		~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		std::string FragmentPaht = "../Shaders/simple_shader.frag.spirv";
		std::string VertexPaht = "../Shaders/simple_shader.vert.spirv";

		void Run();

			
	private:
		void LoadGameObjects();

		Window window{ WIDTH, HEIGHT, "Hello Vulkan sRGB!" };
		Device device{ window };
		Renderer renderer{ window, device };
		
		std::vector<GameObject>	gameObjects;
	};
}