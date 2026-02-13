#include "Renderer.h"
#include "Transform.h"
#include "YamlSerializer.h"
#include "Material.h"
#include "FBXManager.h"
#include "MaterialManager.h"
#include "ComponentImplement.inl"
#include "Entity.h"
#include "RendererSystem.h"
#include "Drawable.h"
#include "Mathf.h"

using namespace LindaEngine;

DYNAMIC_CREATE(MeshRenderer)
DYNAMIC_CREATE(SkinMeshRenderer)

Renderer::Renderer(Entity& entity, bool enable) : Component(entity, enable)
{
	RendererSystem::Add(this);
}

Renderer::~Renderer()
{
	RendererSystem::Remove(this);
}

bool Renderer::Serialize()
{
	YAML::Emitter& out = *YamlSerializer::out;

	out << YAML::Value << YAML::BeginMap;

	out << YAML::Key << "enable" << YAML::Value << _enable;
	out << YAML::Key << "ShadowCast" << YAML::Value << _shadowCast;
	out << YAML::Key << "ReceiveShadow" << YAML::Value << _receiveShadow;
	if (nullptr != _mesh)
	{
		out << YAML::Key << "Mesh";
		_mesh->Serialize();
	}
	if (_materialList.size() > 0)
	{
		out << YAML::Key << "Material";
		out << YAML::Value << YAML::BeginSeq;
		for (auto& mat : _materialList)
		{
			if (nullptr == mat)
				continue;
			mat->Serialize();
		}
		out << YAML::EndSeq;
	}

	return true;
}

bool Renderer::Deserialize(YAML::Node& node)
{
	_enable = node["enable"].as<bool>();
	_shadowCast = node["ShadowCast"].as<bool>();
	_receiveShadow = node["ReceiveShadow"].as<bool>();

	auto mesh = node["Mesh"];
	auto materials = node["Material"];
	
	if (mesh)
	{
		_mesh = FBXManager::GetMesh(mesh["FilePath"].as<std::string>().c_str(), mesh["HashCode"].as<std::string>().c_str());
		_mesh->Deserialize(mesh);
	}

	for (auto mat : materials)
	{
		Ref<Material> pointer = MaterialManager::GetMaterial(mat["FilePath"].as<std::string>().c_str());
		_materialList.push_back(pointer);
		pointer->Deserialize(mat);
	}

	FillDrawables();
	return true;
}

void Renderer::SetMesh(Ref<Mesh> mesh)
{
	_mesh = mesh;
	_materialList.clear();
	FillDrawables();
}

void Renderer::AddMaterial(int index, Ref<Material> mat)
{
	_materialList.insert(_materialList.begin() + index, mat);

	if (_type == RenderComponentType::Skybox || _type == RenderComponentType::ScreenTriangle)
		return;
	_drawables[index]->material = mat;
}

void Renderer::TransformDirty()
{
	glm::vec3 min = _mesh->GetBoundingBox().min;
	glm::vec3 max = _mesh->GetBoundingBox().max;

	std::vector<glm::vec3> vertices = {
		glm::vec3(min.x, min.y, min.z),
		glm::vec3(max.x, min.y, min.z),
		glm::vec3(min.x, max.y, min.z),
		glm::vec3(max.x, max.y, min.z),
		glm::vec3(min.x, min.y, max.z),
		glm::vec3(max.x, min.y, max.z),
		glm::vec3(min.x, max.y, max.z),
		glm::vec3(max.x, max.y, max.z)
	};

	const glm::mat4& transform = _transform->GetLocalToWorldMat();

	_aabb.min = glm::vec3(Mathf::MaxValue);
	_aabb.max = glm::vec3(Mathf::MinValue);

	for (const auto& vertex : vertices) {
		glm::vec4 transformed = transform * glm::vec4(vertex, 1.0f);
		glm::vec3 pos = glm::vec3(transformed) / transformed.w;

		_aabb.min = glm::min(_aabb.min, pos);
		_aabb.max = glm::max(_aabb.max, pos);
	}

	//_aabb.min = _transform->GetLocalToWorldMat() * glm::vec4(_mesh->GetBoundingBox().min, 1.0f);
	//_aabb.max = _transform->GetLocalToWorldMat() * glm::vec4(_mesh->GetBoundingBox().max, 1.0f);
	_aabb.CalculateCenterSize();
}

bool Renderer::CanRender(int index, int minQueue, int maxQueue)
{
	if (_materialList.size() <= index)
		return false;

	return _materialList[index]->CanRender(Material::overrideLightMode, minQueue, maxQueue);
}

bool Renderer::InLayerMask(int layer)
{
	if (-1 == layer)
		return false;
	if (0 == layer)
		return true;
	return _entity.GetLayer() & layer;
}

Drawable& Renderer::GetSkyboxRenderer()
{
	static bool isLoaded = false;
	static Drawable drawable;
	if (isLoaded == false)
	{
		drawable.meshData = FBXManager::GetSkybox()->GetMeshData();
		drawable.transform = nullptr;
		isLoaded = true;
	}
	return drawable;
}

void Renderer::SetSkyboxMaterial(Ref<Material> material)
{
	GetSkyboxRenderer().material = material;
}

Ref<Material> Renderer::GetSkyboxMaterial()
{
	return GetSkyboxRenderer().material;
}

void Renderer::RenderSkybox()
{
	Material::overrideLightMode = "Skybox";
	GetSkyboxRenderer().Draw();
}

Drawable& Renderer::GetBoundingBoxRenderer()
{
	static bool isLoaded = false;
	static Drawable drawable;
	if (isLoaded == false)
	{
		drawable.meshData = FBXManager::GetBoundingBox()->GetMeshData();
		static Entity root("BoundingBox-Root"); root.SetDontDestory(true);
		static Entity child("BoundingBox-Child"); child.SetDontDestory(true);
		child.GetTransform()->SetParent(root.GetTransform());
		drawable.transform = child.GetTransform();
		drawable.material = MaterialManager::GetMaterialByShader("BuiltInAssets/Shaders/BoundingBox.shader");
		isLoaded = true;
	}
	return drawable;
}

void Renderer::RenderBoundingBox()
{
	Material::overrideLightMode = "Adjunct";
	Drawable& drawable = GetBoundingBoxRenderer();
	drawable.transform->SetLocalPosition(_transform->GetWorldScale() * _mesh->GetBoundingBox().center);
	drawable.transform->SetWorldScale(_transform->GetWorldScale() * _mesh->GetBoundingBox().size);
	drawable.transform->Tick();
	Transform* parent = (Transform*)drawable.transform->GetParent();
	parent->SetWorldPosition(_transform->GetWorldPosition());
	parent->SetWorldRotation(_transform->GetWorldRotation());
	parent->Tick();
	drawable.Draw();
}

void Renderer::FillDrawables()
{
	if (_type == RenderComponentType::Skybox || _type == RenderComponentType::ScreenTriangle)
		return;

	_drawables.clear();

	int index = 0;
	for (auto& data : _mesh->GetAllMeshData())
	{
		Ref<Drawable> da = CreateRef<Drawable>();
		da->meshData = &data;
		da->transform = _transform;
		if (_materialList.size() > index)
			da->material = _materialList[index];
		else
			da->material = nullptr;
		index++;
		_drawables.push_back(da);
	}
}

/////////////////////////////////////////////////////////////////////

MeshRenderer::MeshRenderer(Entity& entity, bool enable) : Renderer(entity, enable)
{
	_type = RenderComponentType::Mesh;
}

MeshRenderer::~MeshRenderer()
{

}

bool MeshRenderer::Serialize()
{
	Renderer::Serialize();

	YAML::Emitter& out = *YamlSerializer::out;
	out << YAML::Key << "Name" << YAML::Value << "MeshRenderer";
	out << YAML::EndMap;

	return true;
}

bool MeshRenderer::Deserialize(YAML::Node& node)
{
	Renderer::Deserialize(node);
	return true;
}

/////////////////////////////////////////////////////////////////////

SkinMeshRenderer::SkinMeshRenderer(Entity& entity, bool enable) : Renderer(entity, enable)
{
	_type = RenderComponentType::SkinMesh;
}

SkinMeshRenderer::~SkinMeshRenderer()
{
}

bool SkinMeshRenderer::Serialize()
{
	Renderer::Serialize();

	YAML::Emitter& out = *YamlSerializer::out;
	out << YAML::Key << "Name" << YAML::Value << "SkinMeshRenderer";
	out << YAML::EndMap;

	return true;
}

bool SkinMeshRenderer::Deserialize(YAML::Node& node)
{
	Renderer::Deserialize(node);
	return true;
}


