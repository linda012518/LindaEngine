#include "Renderer.h"

using namespace LindaEngine;

Renderer::Renderer()
{
	std::cout << "Renderer" << _selfID << std::endl;
}

Renderer::~Renderer()
{
	std::cout << "~Renderer" << _selfID << std::endl;
}
