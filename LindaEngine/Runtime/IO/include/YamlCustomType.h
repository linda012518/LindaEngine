#pragma once

#include "yaml-cpp/yaml.h"
#include "RenderState.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace LindaEngine;

namespace YAML
{
	template<>
	struct convert<ColorMask>
	{
		static Node encode(const ColorMask& cm)
		{
			Node node;
			node.push_back(cm.R);
			node.push_back(cm.G);
			node.push_back(cm.B);
			node.push_back(cm.A);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, ColorMask& cm)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			cm.R = node[0].as<bool>();
			cm.G = node[1].as<bool>();
			cm.B = node[2].as<bool>();
			cm.A = node[3].as<bool>();
			return true;
		}
	};

	template<>
	struct convert<Color>
	{
		static Node encode(const Color& c)
		{
			Node node;
			node.push_back(c.r);
			node.push_back(c.g);
			node.push_back(c.b);
			node.push_back(c.a);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, Color& c)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			c.r = node[0].as<float>();
			c.g = node[1].as<float>();
			c.b = node[2].as<float>();
			c.a = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& c)
		{
			Node node;
			node.push_back(c.r);
			node.push_back(c.g);
			node.push_back(c.b);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& c)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			c.r = node[0].as<float>();
			c.g = node[1].as<float>();
			c.b = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& c)
		{
			Node node;
			node.push_back(c.r);
			node.push_back(c.g);
			node.push_back(c.b);
			node.push_back(c.a);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& c)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			c.r = node[0].as<float>();
			c.g = node[1].as<float>();
			c.b = node[2].as<float>();
			c.a = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::ivec4>
	{
		static Node encode(const glm::ivec4& c)
		{
			Node node;
			node.push_back(c.r);
			node.push_back(c.g);
			node.push_back(c.b);
			node.push_back(c.a);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::ivec4& c)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			c.r = node[0].as<int>();
			c.g = node[1].as<int>();
			c.b = node[2].as<int>();
			c.a = node[3].as<int>();
			return true;
		}
	};

	template<>
	struct convert<glm::quat>
	{
		static Node encode(const glm::quat& c)
		{
			Node node;
			node.push_back(c.x);
			node.push_back(c.y);
			node.push_back(c.z);
			node.push_back(c.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::quat& c)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			c.x = node[0].as<float>();
			c.y = node[1].as<float>();
			c.z = node[2].as<float>();
			c.w = node[3].as<float>();
			return true;
		}
	};
}

inline YAML::Emitter& operator<<(YAML::Emitter& out, const Color& c)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << c.r << c.g << c.b << c.a << YAML::EndSeq;
	return out;
}

inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& c)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << c.r << c.g << c.b << YAML::EndSeq;
	return out;
}

inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& c)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << c.r << c.g << c.b << c.a << YAML::EndSeq;
	return out;
}

inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::ivec4& c)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << c.r << c.g << c.b << c.a << YAML::EndSeq;
	return out;
}

inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::quat& c)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << c.x << c.y << c.z << c.w << YAML::EndSeq;
	return out;
}

inline YAML::Emitter& operator<<(YAML::Emitter& out, const ColorMask& cm)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << cm.R << cm.G << cm.B << cm.A << YAML::EndSeq;
	return out;
}
