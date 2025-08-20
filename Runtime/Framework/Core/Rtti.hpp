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

		//�Ƿ�Ϊͬһ����
		bool IsSameType(const Rtti& type) const;
		//�Ƿ�Ϊ����
		bool IsDerived(const Rtti& type) const;
		//��ȡ����Rtti
		Rtti* GetBase() const;

	private:
		std::string _className;
		Rtti* _pBase;//����Rtti

	};
}
