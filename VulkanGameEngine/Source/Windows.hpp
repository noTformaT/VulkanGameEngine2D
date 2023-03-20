#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace Engine 
{

	class Window
	{
	public:
		Window(int width, int height, std::string name);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		bool ShouldClose();
		VkExtent2D GetExtend();
		bool WasWindowResized();
		void ResetWindowResized();

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		static void FrameBufferResizeCallback(GLFWwindow* window, int width, int height);

		void initWindow();

		int width;
		int height;
		bool frameBufferResized = false;

		std::string windowName;

		GLFWwindow* window;
	};
}