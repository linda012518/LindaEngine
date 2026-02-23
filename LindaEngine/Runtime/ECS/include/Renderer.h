#pragma once

#include "AutoPtr.h"
#include "Component.h"
#include "BoundingBox.h"
#include "RenderEnumData.h"

#include <vector>

namespace LindaEngine
{
	class Material;
	class Mesh;
	class Transform;
	struct Drawable;
	struct BoneData;

	class Renderer : public Component
	{
	public:
		Renderer(Entity& entity, bool enable = true);
		virtual ~Renderer();

		bool Serialize();
		bool Deserialize(YAML::Node& node);

		void SetMesh(Ref<Mesh> mesh);
		virtual void AddMaterial(int index, Ref<Material> mat);
		void TransformDirty();
		bool IsSkybox() { return _type == RenderComponentType::Skybox; }
		bool CanRender(int index, int minQueue, int maxQueue);
		bool HasError(int index);
		bool InLayerMask(int layer);
		void SetDistanceToCamera(float distance) { _distanceToCamera = distance; }
		float GetDistanceToCamera() { return _distanceToCamera; }
		Ref<Drawable> GetDrawable(int index) { return _drawables[index]; }

		static Drawable& GetSkyboxRenderer();
		static void SetSkyboxMaterial(Ref<Material> material);
		static Ref<Material> GetSkyboxMaterial();
		static void RenderSkybox();

		static Drawable& GetBoundingBoxRenderer();
		void RenderBoundingBox();

		Mesh* GetMesh() { return _mesh.get(); }
		RenderComponentType GetType() { return _type; }
		AABBBoundingBox& GetBoundingBox() { return _aabb; }
		std::vector<Ref<Material>>& GetMaterials() { return _materialList; }

	protected:
		void FillDrawables();

	protected:
		std::vector<Ref<Material>> _materialList;
		Ref<Mesh> _mesh;
		bool _shadowCast = true; //是否投射阴影
		bool _receiveShadow = true; //是否接收阴影
		RenderComponentType _type = RenderComponentType::None;
		AABBBoundingBox _aabb;//这里分根据Transform做变换，其实是OBB了
		float _distanceToCamera = 0.0f;
		std::vector<Ref<Drawable>> _drawables;
	};

	class MeshRenderer : public Renderer
	{
	public:
		DECLARE_DYNAMIC_CREATE()
		MeshRenderer(Entity& entity, bool enable = true);
		virtual ~MeshRenderer();

		bool Serialize();
		bool Deserialize(YAML::Node& node);
	};

	class SkinMeshRenderer : public Renderer
	{
	public:
		DECLARE_DYNAMIC_CREATE()
		SkinMeshRenderer(Entity& entity, bool enable = true);
		virtual ~SkinMeshRenderer();

		void Tick();

		bool Serialize();
		bool Deserialize(YAML::Node& node);

		void AddMaterial(int index, Ref<Material> mat) override;

		void SetBones(std::vector<Transform*> bones);
		std::vector<Transform*>& GetBones();
		void SetRootBone(Transform* root);
		Transform* GetRootBone();

		void SetBonesData(std::vector<BoneData> data);
		std::vector<BoneData>& GetBonesData();

	private:
		Transform* _rootBone = nullptr;
		std::vector<Transform*> _bones;
		std::vector<BoneData> _bonesData;
		std::vector<glm::mat4> _boneMatrices;
	};
}
