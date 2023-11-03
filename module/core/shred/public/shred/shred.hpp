#pragma once

#include "typeSystem/qStructDef.hpp"
#include "typeSystem/qObjDef.hpp"
#include "typeSystem/qObj.hpp"
#include "typeSystem/fieldType/all.hpp"
#include "typeSystem/macros.hpp"
#include "typeSystem/structMacros.hpp"


GEN_QSTRUCT(ShredLocalIdentifier)
{
    GEN_QSTRUCT_BODY(ShredLocalIdentifier)

    std::string name {"Shred"};
    int duplicationIndex {0};

    std::string getUniqueName() const { return name +"_"+ std::to_string(duplicationIndex);};
};

GEN_QOBJ(Shred,QObj)
{
    GEN_QOBJ_BODY(Shred,QObj);
    GEN_QOBJ_DEF_CONSTRUCTOR_AND_DESTRUCTOR(Shred,QObj);
public:
    static OwnerPtr<Shred> ConstructRoot(WeakPtr<QObjDef> in_class);

public:
    WeakPtr<Shred> appendChildren(WeakPtr<QObjDef> in_class, std::string in_name,const std::function<void(WeakPtr<Shred>)>& in_beforePropagateRegistration = [](auto ptr){});
    std::string getUniqueName() {return id.getUniqueName(); }
public:
    void postLoad() override;


protected:
    ShredLocalIdentifier id;
    WeakPtr<Shred> directParent {};
    DynamicArray<OwnerPtr<Shred>> children {};
};