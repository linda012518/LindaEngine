#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "assimp-vc143-mtd.lib")

#include <iostream>

#include "Application.h"

#include "Entity.h"
#include "Transform.h"
#include "TransformSystem.h"
#include "Camera.h"
#include "CameraSystem.h"
#include "Scene.h"
#include "ShaderManager.h"
#include "YamlSerializer.h"
#include "Material.h"
#include "MaterialPass.h"
#include "Texture.h"
#include "Path.h"
#include "SceneManagerEditor.h"
#include "NodeEditor.h"
#include <typeinfo>
#include "SceneManager.h"
#include "ComponentSystem.h"
#include "Renderer.h"
#include "MeshManager.h"
#include "MaterialManager.h"
#include "MaterialManagerEditor.h"

using namespace LindaEngine;
using namespace LindaEditor;

#define LindaEditor

void SerializeMaterial()
{
	//Ref<Material> mat = CreateRef<Material>();
	//mat->_renderType = RenderType::Transparent;
	//mat->_renderQueue = 3000;

	//mat->_depthPass = CreateRef<MaterialPass>();
	//mat->_depthPass->_lightMode = "Depth";
	//mat->_depthNormalPass = CreateRef<MaterialPass>();
	//mat->_depthNormalPass->_lightMode = "DepthNormal";
	//mat->_shadowCasterPass = CreateRef<MaterialPass>();
	//mat->_shadowCasterPass->_lightMode = "ShadowCaster";
	//mat->_colorPasses.push_back(CreateRef<MaterialPass>());
	//mat->_colorPasses.push_back(CreateRef<MaterialPass>());

	//mat->_colorPasses[1]->_lightMode = "customLightMode";
	//mat->_colorPasses[1]->_keywords.push_back("NORMAL_MAP");
	//mat->_colorPasses[1]->_keywords.push_back("LogDepth");
	//mat->_colorPasses[1]->_renderState.colorMask.A = false;
	//mat->_colorPasses[1]->_renderState.blendState.enableBlend = true;
	//mat->_colorPasses[1]->_renderState.blendState.blendSrc = BlendFunc::ONE_MINUS_DST_ALPHA;
	//mat->_colorPasses[1]->_renderState.cullFaceFunc = CullFaceFunc::None;
	//mat->_colorPasses[1]->_renderState.depthState.depthTest = false;
	//mat->_colorPasses[1]->_renderState.polygonModeState.polygonMode = PolygonMode::LINE;
	//mat->_colorPasses[1]->_renderState.stencilState.stencilTest = true;
	//mat->_colorPasses[1]->_renderState.stencilState.stencilRef = 6;
	//Ref<TextureUniformData> d1 = CreateRef<TextureUniformData>();
	//d1->value = "texturl/aa/bb";
	//d1->name = "MainTex";
	//mat->_colorPasses[1]->_uniformNameMap[d1->name] = d1;
	//Ref<FloatUniformData> d2 = CreateRef<FloatUniformData>();
	//d2->value = 3.6f;
	//d2->name = "Range";
	//mat->_colorPasses[1]->_uniformNameMap[d2->name] = d2;
	//Ref<Int4UniformData> d3 = CreateRef<Int4UniformData>();
	//d3->value.z = 8;
	//d3->name = "Vector";
	//mat->_colorPasses[1]->_uniformNameMap[d3->name] = d3;

	//Material::overrideMat = mat;
	//YamlSerializer::SerializeMaterial("Assets/Materials/test.mat");
}

void SerializeScene0()
{
	Ref<Scene> scene = SceneManagerEditor::GetCurrentScene();

	Entity* entity = scene->CreateEntity("test1");
	entity->GetTransform()->Tick();

	Entity* entity2 = scene->CreateEntity("test2");
	entity2->GetTransform()->SetParent(entity->GetTransform());
	entity2->GetTransform()->SetLocalEulerAngles(glm::vec3(10, 10, 10));
	entity2->GetTransform()->Tick();
	entity2->SetActive(false);
	PerspectiveCamera* camera = entity2->AddComponent<PerspectiveCamera>();

	Entity* entity3 = scene->CreateEntity("test3");
	MeshRenderer* renderer = entity3->AddComponent<MeshRenderer>();
	renderer->SetMesh(MeshManager::GetMesh("Sphere"));
	renderer->SetMesh(MeshManager::GetMesh("Assets/Meshs/rock.obj"));
	renderer->AddMaterial(0, MaterialManager::GetMaterial("Assets/Materials/test.mat"));

	SceneManagerEditor::SaveScene();//"Assets/Scenes/All0.scene"
	SceneManagerEditor::AddToBuild(0, SceneManagerEditor::GetCurrentNode());
}

void SerializeTexture()
{
	Ref<Texture2D> tex2d = CreateRef<Texture2D>();
	tex2d->path = "Assets/Maps/wall.jpg";
	tex2d->isDataSRGB = true;
	tex2d->mipmapCount = 6;
	tex2d->filter = FilterMode::Point;
	tex2d->warpU = TextureWrapMode::Repeat;
	tex2d->warpV = TextureWrapMode::Repeat;
	tex2d->warpW = TextureWrapMode::Repeat;
	Texture::overrideTexture = tex2d;
	YamlSerializer::DeSerializeTexture("Assets/Maps/Yamls/go.texture");

	Ref<Cubemap> cubemap = CreateRef<Cubemap>();
	cubemap->isDataSRGB = false;
	cubemap->mipmapCount = 6;
	cubemap->filter = FilterMode::Point;
	cubemap->warpU = TextureWrapMode::Repeat;
	cubemap->warpV = TextureWrapMode::Repeat;
	cubemap->warpW = TextureWrapMode::Repeat;
	cubemap->left = "left";
	cubemap->right = "right";
	cubemap->top = "top";
	cubemap->bottom = "bottom";
	cubemap->front = "front";
	cubemap->back = "back";
	cubemap->srcType = CubemapSrcType::SixTexture;
	Texture::overrideTexture = cubemap;
	YamlSerializer::DeSerializeTexture("Assets/Maps/Yamls/testCube.texture");
}

int main()
{
	//Ref<SceneNodeEditor> sne0 = CreateRef<SceneNodeEditor>();
	//sne0->name = "test0.scene"; 
	//sne0->path = "go/go/test0.scene";

	//Ref<SceneNodeEditor> sne1 = CreateRef<SceneNodeEditor>();
	//sne1->name = "test1.scene";
	//sne1->path = "go/go/test1.scene";

	//SceneManagerEditor::AddToBuild(0, sne1);
	//SceneManagerEditor::AddToBuild(1, sne0);
	//SceneManagerEditor::Build("Assets/Config/SceneConfig");

	//SerializeMaterial();
	//return 0;

	//SerializeScene0();
	//SceneManagerEditor::Build("Assets/Config/SceneConfig");

	//SceneManager::Initialize();
	//Ref<Scene> scene = SceneManager::LoadScene(0);
	//ComponentSystem::OnDeserializeFinish();
	//ComponentSystem::Tick();

	//int a = 0;
	//return 0;

	Ref<ShaderNodeEditor> node = MaterialManagerEditor::GetCurrentShaderNode();
	node->name = "test.shader";
	node->path = "Assets/Shaders/test.shader";
	MaterialManagerEditor::CreateMaterial("");


	Application app;
	app.Initialize();
	app.Tick();
	app.Finalize();

	return 0;
}