#pragma once

namespace LindaEngine
{
	class ISerializable
	{
	public:
		virtual void Serialize() = 0;

		virtual bool Deserialize() = 0;

		virtual ~ISerializable() = default;
	};
}