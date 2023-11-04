#pragma once
#include "template/memory/weakPtr.hpp"
#include "qObjDef.hpp"

class RuntimeTypeInformation
{
public:
    void registerQObjDef(const WeakPtr<QObjDef> inQStructTypes)
    {
        LOG_INF("registering type {}", inQStructTypes->name);
        QObjDef[inQStructTypes->name] = inQStructTypes;
    }
    const WeakPtr<QObjDef> getDynamicType(const std::string& inTypeName) const
    {
        return QObjDef[inTypeName];
    }

protected:
    mutable std::unordered_map<std::string,WeakPtr<QObjDef>> QObjDef;
};