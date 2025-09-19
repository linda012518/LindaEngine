#pragma once

namespace LindaEngine
{
	template<typename T>
	class Singleton
	{
	public:
		static T& Instance()
		{
			static T _instance;
			return _instance;
		}

		Singleton(const Singleton&) = delete;
		Singleton(Singleton&&) = delete;
		Singleton& operator= (const Singleton&) = delete;
		Singleton& operator=(Singleton&&) = delete;

		virtual ~Singleton() { }

	protected:
		Singleton() { }
	};
}

