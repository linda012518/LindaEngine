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

	class Renderer : public Component
	{
	public:
		Renderer(Entity& entity, bool enable = true);
		virtual ~Renderer();

		bool Serialize();
		bool Deserialize(YAML::Node& node);

		void SetMesh(Ref<Mesh> mesh);
		void AddMaterial(int index, Ref<Material> mat);
		void Render();
		void TransformDirty();
		bool CanRender(int index, int layer, int minQueue, int maxQueue);
		void SetDistanceToCamera(float distance) { _distanceToCamera = distance; }
		float GetDistanceToCamera() { return _distanceToCamera; }
		Ref<Drawable> GetDrawable(int index) { return _drawables[index]; }

		static Renderer* GetSkyboxRenderer();
		static void SetSkyboxMaterial(Ref<Material> material);
		static Ref<Material> GetSkyboxMaterial();
		static void RenderSkybox();

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
		AABBBoundingBox _aabb;
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

		bool Serialize();
		bool Deserialize(YAML::Node& node);
	};

	class SkyboxRenderer : public Renderer
	{
	public:
		DECLARE_DYNAMIC_CREATE()
		SkyboxRenderer(Entity& entity, bool enable = true);
		virtual ~SkyboxRenderer();

		bool Serialize();
		bool Deserialize(YAML::Node& node);

	};
}
