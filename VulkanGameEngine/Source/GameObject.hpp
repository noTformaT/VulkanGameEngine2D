#pragma once

#include "Model.hpp"

#include <memory>

namespace Engine
{

	struct Transform2DComponent
	{
		glm::vec2 translation{};
		glm::vec2 scale{ 1.0f, 1.0f };
		float rotation;
		
		glm::mat2 mat2();
	};

	class GameObject
	{
	public:
		using id_t = unsigned int;
		
		static GameObject CreateGameObject();

		id_t GetID();

		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject(GameObject&&) = default;
		GameObject& operator=(GameObject&&) = default;

		std::shared_ptr<Model> model{};
		glm::vec3 color;
		Transform2DComponent transform2d{};

		//~GameObject();

	private:
		GameObject(id_t objId);

		id_t id;
	};
}