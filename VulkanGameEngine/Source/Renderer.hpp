#pragma once

#include "Windows.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace Engine
{

	class Renderer
	{
	public:
		Renderer(Window& win, Device& dev);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		std::string FragmentPaht = "../Shaders/simple_shader.frag.spirv";
		std::string VertexPaht = "../Shaders/simple_shader.vert.spirv";

		VkCommandBuffer BeginFrame();
		void EndFrame();

		void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

		VkRenderPass GetSwapChainRenderPass() const;

		bool IsFrameInProgress() const;
		VkCommandBuffer GetCurrentCommandBuffer() const;

		int GetFrameIndex() const;

	private:
		void CreateCommandBuffers();
		void FreeCommandBuffers();
		void RecreateSwapChain();

		Window& window;
		Device& device;

		std::unique_ptr<SwapChain> swapChain;
		std::vector<VkCommandBuffer> commandsBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex{ 0 };
		bool isFrameStarted;
	};
}