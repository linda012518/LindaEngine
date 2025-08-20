#include "Rtti.hpp"

using namespace LindaEngine;

Rtti::Rtti(std::string& className, Rtti* pBase, CreateObjectFun cf)
{
    _className = className;
}

Rtti::~Rtti()
{
}

bool Rtti::IsSameType(const Rtti& type) const
{
    return &type == this;
}

bool Rtti::IsDerived(const Rtti& type) const
{
    const Rtti* temp = this;
    while (false == temp->IsSameType(type))
    {
        if (temp->_pBase)
        {
            temp = temp->_pBase;
        }
        else
        {
            return false;
        }
    }
    return true;
}

Rtti* Rtti::GetBase() const
{
    return _pBase;
}
