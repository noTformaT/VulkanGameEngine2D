#include "Application.hpp"


#include "SimpleRenderSystem.hpp"

#include <stdexcept>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


namespace Engine
{

	Application::Application()
	{
		LoadGameObjects();

	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		SimpleRenderSystem simpleRenderSystem{ device, renderer.GetSwapChainRenderPass() };

		while (!window.ShouldClose())
		{
			glfwPollEvents();
			
			if (VkCommandBuffer commandBuffer = renderer.BeginFrame())
			{
				renderer.BeginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.RenderGameObjects(commandBuffer, gameObjects);
				renderer.EndSwapChainRenderPass(commandBuffer);
				renderer.EndFrame();
			}
		}
		vkDeviceWaitIdle(device.GetDevice());
	}

	void Application::LoadGameObjects()
	{
		std::vector<Model::Vertex> vertices{ 
			{{ 0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{ 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}} };

		std::shared_ptr<Model> model = std::make_shared<Model>(device, vertices);

		GameObject triangle = GameObject::CreateGameObject();
		triangle.model = model;
		triangle.color = { 0.1f, 0.8f, 0.1f };
		triangle.transform2d.translation.x = .2f;
		triangle.transform2d.scale = { 2.f, 0.5f };
		triangle.transform2d.rotation = 0.25f * glm::two_pi<float>();

		gameObjects.push_back(std::move(triangle));
	}

}