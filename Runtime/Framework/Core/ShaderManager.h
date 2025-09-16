#pragma once

#include <unordered_map>
#include <string>

#include "Common/Singleton.h"

namespace LindaEngine
{
	struct ShaderSource
	{
		std::string vertex;
		std::string fragment;
	};

	class ShaderManager : public Singleton<ShaderManager>
	{

	public:
		void AddShaderSource(std::string& path, std::string& vert, std::string frag);
		
	private:
		std::unordered_map<std::string, ShaderSource> _map;
	};
}
