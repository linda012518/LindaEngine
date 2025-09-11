#include "Renderer.h"
#include "Core/Mesh.h"
#include "Core/Material.h"

using namespace LindaEngine;

Renderer::Renderer()
{
	std::cout << "		Renderer" << _selfID << std::endl;
}

Renderer::~Renderer()
{
	std::cout << "		~Renderer" << _selfID << std::endl;
}

void Renderer::SetMesh(Mesh* m)
{
	_mesh = m;
}

void Renderer::AddMaterial(Material* mat)
{
	_materials.push_back(mat);
}

void Renderer::Render()
{
}
