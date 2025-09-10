#pragma once

#include "Core/Component.hpp"

#include "glm/glm.hpp"

namespace LindaEngine
{
	class Camera : public Component
	{
	public:
		Camera(const glm::mat4& projectMat);
		virtual ~Camera();

		const glm::mat4& GetViewMatrix();

		virtual const glm::mat4& GetProjectMatrix();

		virtual const glm::mat4& GetViewProjectMatrix();

		virtual void Awake();

		void Tick();

		virtual void TransformChange();

	private:
		void UpdateMatrix();

	private:
		glm::mat4 _viewMatrix;
		glm::mat4 _projectMatrix;
		glm::mat4 _viewProjectMatrix;

		glm::mat4 _viewInverseMatrix;
		glm::mat4 _projectInverseMatrix;
		glm::mat4 _viewProjectInverseMatrix;
	};
}
