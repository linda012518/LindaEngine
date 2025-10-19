#pragma once

namespace YAML { class Node; }

namespace LindaEngine
{
	class ISerializable
	{
	public:
		virtual bool Serialize() = 0;

		virtual bool Deserialize(YAML::Node& node) = 0;

		virtual ~ISerializable() = default;
	};
}