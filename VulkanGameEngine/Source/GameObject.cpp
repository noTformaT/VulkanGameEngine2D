#include "GameObject.hpp"


namespace Engine
{
	using id_t = unsigned int;

	GameObject GameObject::CreateGameObject()
	{
		static id_t currentId = 0;
		return GameObject(currentId++);
	}

	id_t GameObject::GetID()
	{
		return id;
	}

	GameObject::GameObject(id_t objId) : id{ objId }
	{
	}

	glm::mat2 Transform2DComponent::mat2()
	{
		const float s = glm::sin(rotation);
		const float c = glm::cos(rotation);
		glm::mat2 rotationMat{ {c, s}, {-s, c} };


		glm::mat2 scaleMat{ {scale.x, 0.0f}, {0.0f, scale.y} };

		return rotationMat * scaleMat;
	}

}