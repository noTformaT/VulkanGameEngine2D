#include "Renderer.hpp"

#include <stdexcept>
#include <array>

namespace Engine
{

	Renderer::Renderer(Window& win, Device& dev) : window{ win }, device{ dev }
	{
		RecreateSwapChain();
		CreateCommandBuffers();
	}

	Renderer::~Renderer()
	{
		FreeCommandBuffers();
	}

	VkCommandBuffer Renderer::BeginFrame()
	{
		assert(!isFrameStarted && "Can't call BeginFrame while already in progress");

		auto result = swapChain->AcquireNextImage(&currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to acquire swap chain image!");
		}

		isFrameStarted = true;

		VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to begin recording command buffer!");
		}

		return commandBuffer;
	}

	void Renderer::EndFrame()
	{
		assert(isFrameStarted && "Can't call EndFrame while already in progress");

		VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();

		if (vkEndCommandBuffer(commandBuffer))
		{
			throw std::runtime_error("Failed to record command buffer!");
		}

		VkResult result = swapChain->SubmitCommandBuffers(&commandBuffer, &currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.WasWindowResized())
		{
			window.ResetWindowResized();
			RecreateSwapChain();
		} else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to present swap chain image!");
		}

		isFrameStarted = false;
		currentFrameIndex = (currentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void Renderer::BeginSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "Can't call BeginSwapChainRenderPass while already in progress");
		assert(commandBuffer == GetCurrentCommandBuffer() && "Can't beging render pass on command buffer from a different frame");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = swapChain->GetRenderPass();
		renderPassInfo.framebuffer = swapChain->GetFrameBuffer(currentImageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChain->GetSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(swapChain->GetSwapChainExtent().width);
		viewport.height = static_cast<float>(swapChain->GetSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, swapChain->GetSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void Renderer::EndSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "Can't call EndSwapChainRenderPass while already in progress");
		assert(commandBuffer == GetCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame");

		vkCmdEndRenderPass(commandBuffer);
	}

	VkRenderPass Renderer::GetSwapChainRenderPass() const
	{
		return swapChain->GetRenderPass();
	}
	bool Renderer::IsFrameInProgress() const
	{
		return isFrameStarted;
	}

	VkCommandBuffer Renderer::GetCurrentCommandBuffer() const
	{
		assert(isFrameStarted && "Cannot get Command Buffer when frame not in progress");
		return commandsBuffers[currentFrameIndex];
	}

	int Renderer::GetFrameIndex() const
	{
		assert(isFrameStarted && "Cannot get Frame Index when frame not in progress");
		return currentFrameIndex;
	}

	void Renderer::CreateCommandBuffers()
	{
		commandsBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = device.GetCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandsBuffers.size());

		if (vkAllocateCommandBuffers(device.GetDevice(), &allocInfo, commandsBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffers!");
		}
	}

	void Renderer::FreeCommandBuffers()
	{
		vkFreeCommandBuffers(device.GetDevice(),
			device.GetCommandPool(),
			static_cast<uint32_t>(commandsBuffers.size()),
			commandsBuffers.data());
		commandsBuffers.clear();
	}

	void Renderer::RecreateSwapChain()
	{
		auto extent = window.GetExtend();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = window.GetExtend();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(device.GetDevice());

		if (swapChain == nullptr)
		{
			swapChain = std::make_unique<SwapChain>(device, extent);
		}
		else
		{
			std::shared_ptr<SwapChain> oldSwapChain = std::move(swapChain);

			swapChain = std::make_unique<SwapChain>(device, extent, oldSwapChain);


			if (!oldSwapChain->CompareSwapFormats(*swapChain.get()))
			{
				throw std::runtime_error("Swap Chain image(or depth) format has changed!");
			}

		}

		// CreatePipeline();
	}
}