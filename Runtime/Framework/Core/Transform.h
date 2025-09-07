#pragma once

#include "LObject.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <list>

namespace LindaEngine
{
	class Entity;
	class Transform : public LObject
	{
	protected:
		glm::vec3 _localPosition = glm::vec3(0.0f);
		glm::vec3 _localEulerAngles = glm::vec3(0.0f);
		glm::quat _localRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 _localScale = glm::vec3(1.0f);

		glm::vec3 _worldPosition = glm::vec3(0.0f);
		glm::vec3 _worldEulerAngles = glm::vec3(0.0f);
		glm::quat _worldRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 _worldScale = glm::vec3(1.0f);

		glm::mat4 _localMatrix = glm::mat4(1.0);//相对于父物体的本地矩阵
		glm::mat4 _worldMatrix = glm::mat4(1.0);//世界矩阵
		glm::mat4 _worldInverseMatrix = glm::mat4(1.0);

		bool _localChange = false;
		bool _worldChange = false;

		Entity* _entity = nullptr;
		Transform* _parent = nullptr;
		std::list<Transform*> _children;

	public:
		Transform() = delete;
		Transform(Entity* entity);
		virtual ~Transform();

		const Entity* GetEntity() const;

		const glm::mat4& GetLocalToWorldMat() const;
		const glm::mat4& GetWorldToLocalMat();

		void SetParent(Transform* parent);
		const Transform* GetParent() const;

		void AddChild(Transform* child);
		void RemoveChild(Transform* child);

		void GetWorldDir(glm::vec3& forward, glm::vec3& up, glm::vec3& right);
		void GetLocalDir(glm::vec3& forward, glm::vec3& up, glm::vec3& right);

		const glm::vec3& GetLocalPosition() const;
		const glm::vec3& GetLocalEulerAngles() const;
		const glm::quat& GetLocalRotation() const;
		const glm::vec3& GetLocalScale() const;

		void SetLocalPosition(const glm::vec3& pos);
		void SetLocalEulerAngles(const glm::vec3& angle);
		void SetLocalRotation(const glm::quat& rotation);
		void SetLocalScale(const glm::vec3& scale);

		const glm::vec3& GetWorldScale() const;
		const glm::vec3& GetWorldPosition() const;
		const glm::vec3& GetWorldEulerAngles() const;
		const glm::quat& GetWorldRotation() const;

		void SetWorldScale(const glm::vec3& scale);
		void SetWorldPosition(const glm::vec3& pos);
		void SetWorldEulerAngles(const glm::vec3& angle);
		void SetWorldRotation(const glm::quat& rotation);

		void Tick();

	private:
		void UpdateWhenLocalChange();
		void UpdateWhenWorldChange();

		void NotifyChange();

		void GetDir(glm::quat& rotation, glm::vec3& forward, glm::vec3& up, glm::vec3& right);
		
	};
}
