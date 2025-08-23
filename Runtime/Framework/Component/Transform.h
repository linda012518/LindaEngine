#pragma once

#include "Core/Component.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace LindaEngine
{
	class Transform : public Component
	{
	private:
		glm::vec3 _localPosition;
		glm::vec3 _localEulerAngles;
		glm::quat _localRotation;
		glm::vec3 _localScale;
		glm::vec3 _position;
		glm::vec3 _eulerAngles;
		glm::quat _rotation;
		glm::vec3 _scale;
		glm::mat4x4 _localToWorldMatrix;
		glm::mat4x4 _worldToLocalMatrix;
	public:
		Transform();
		virtual ~Transform();

		const glm::vec3& GetLocalPosition() const;
		void SetLocalPosition(const glm::vec3& pos);

		const glm::vec3& GetLocalEulerAngles() const;
		void SetLocalEulerAngles(const glm::vec3& angle);

		const glm::quat& GetLocalRotation() const;
		void SetLocalRotation(const glm::quat& rotation);

		const glm::vec3& GetLocalScale() const;
		void SetLocalScale(const glm::vec3& scale);

		const glm::vec3& GetWorldScale() const;
		void SetWorldScale(const glm::vec3& scale);

		const glm::vec3& GetWorldPosition() const;
		void SetWorldPosition(const glm::vec3& pos);

		const glm::vec3& GetWorldEulerAngles() const;
		void SetWorldEulerAngles(const glm::vec3& angle);

		const glm::quat& GetWorldRotation() const;
		void SetWorldRotation(const glm::quat& rotation);

	private:
		void CalculateLocalPosition();
		void CalculateWorldPosition();
		void CalculateLocalEulerAngles();
		void CalculateWorldEulerAngles();
		void CalculateLocalRotation();
		void CalculateWorldRotation();
		void CalculateLocalScale();
		void CalculateWorldScale();
		void UpdateEulerAngles();
		void UpdateRotation();
		void UpdateMatrix();
	};
}
