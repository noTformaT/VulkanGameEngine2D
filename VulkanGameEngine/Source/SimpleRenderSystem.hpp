#pragma once

#include "Pipeline.hpp"
#include "Device.hpp"
#include "GameObject.hpp"

#include <memory>
#include <vector>

namespace Engine
{

	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(Device& device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;


		std::string FragmentPaht = "../Shaders/simple_shader.frag.spirv";
		std::string VertexPaht = "../Shaders/simple_shader.vert.spirv";

		void RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects);
	private:
		void CreatePipelineLayout();
		void CreatePipeline(VkRenderPass renderPass);
		
		Device& device;

		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
	};
}