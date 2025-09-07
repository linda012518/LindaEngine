#pragma once

#include "Core/Component.hpp"

#include "glm/glm.hpp"

namespace LindaEngine
{
	class Camera : public Component
	{
	public:
		Camera();
		virtual ~Camera();

		const glm::mat4& GetViewMatrix();

		virtual const glm::mat4& GetProjectMatrix();

		virtual const glm::mat4& GetViewProjectMatrix();

		virtual void OnEvent(void* userData);

		void Tick();

	private:
		glm::mat4 _viewMatrix;
		glm::mat4 _projectMatrix;
		glm::mat4 _viewProjectMatrix;

		bool _change = false;
	};
}
