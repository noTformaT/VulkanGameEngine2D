#include "Windows.hpp"

#include <stdexcept>
#include <iostream>

namespace Engine
{

	Window::Window(int w, int h, std::string name) : width{w}, height{h}, windowName{name}
	{
		initWindow();
	}

	Window::~Window()
	{
		glfwDestroyWindow(window);
		glfwTerminate();

		std::cout << "Windows delete" << std::endl;
	}

	bool Window::ShouldClose()
	{
		return glfwWindowShouldClose(window);
	}

	VkExtent2D Window::GetExtend()
	{
		return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
	}

	bool Window::WasWindowResized()
	{
		return frameBufferResized;
	}

	void Window::ResetWindowResized()
	{
		frameBufferResized = false;
	}

	void Window::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Windows Surface");
		}
	}

	void Window::FrameBufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto engineWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		engineWindow->frameBufferResized = true;
		engineWindow->width = width;
		engineWindow->height = height;
	}

	void Window::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetWindowSizeCallback(window, FrameBufferResizeCallback);
	}
}