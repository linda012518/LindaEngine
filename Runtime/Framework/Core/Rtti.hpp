#pragma once

#include <string>

namespace LindaEngine
{
	class LObject;
	typedef LObject* (*CreateObjectFun)();

	class Rtti
	{
	public:
		Rtti(std::string& className, Rtti* pBase, CreateObjectFun cf);
		~Rtti();

		//是否为同一类型
		bool IsSameType(const Rtti& type) const;
		//是否为父类
		bool IsDerived(const Rtti& type) const;
		//获取父类Rtti
		Rtti* GetBase() const;

	private:
		std::string _className;
		Rtti* _pBase;//父类Rtti

	};
}
