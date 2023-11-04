#include "shred/shred.hpp"

GEN_QSTRUCT_STATIC_DEF(ShredLocalIdentifier,{
    GEN_QSTRUCT_FIELD_ENTRY(ShredLocalIdentifier,name),
    GEN_QSTRUCT_FIELD_ENTRY(ShredLocalIdentifier,duplicationIndex)
});

BEGIN_GEN_QOBJ_STATIC_DEF(Shred,QObj)
DynamicArray<OwnerPtr<Field>>
{
    GEN_QOBJ_DEF_FIELD_ENTRY(Shred,id),
    GEN_QOBJ_DEF_FIELD_ENTRY(Shred,children)
},
DynamicArray<OwnerPtr<Method>>
{

}
END_GEN_QOBJ_STATIC_DEF()

OwnerPtr<Shred> Shred::ConstructRoot(WeakPtr<QObjDef> in_class)
{
    ENSURE(in_class);
    OwnerPtr<Shred> shred;
    shred.unsafe_createInstance(in_class->memorySize, [&](auto in_weak){
        in_class->constructInstance(in_weak);
    });
    shred->id = {{},"root",0};
    shred->children.reserve(8);
    return std::move(shred);
}

WeakPtr<Shred> Shred::appendChildren(WeakPtr<QObjDef> in_class, std::string in_name,
                                     const std::function<void(WeakPtr<Shred>)>& in_beforePropagateRegistration)
{
    ENSURE(in_class);

    OwnerPtr<Shred> shred;

    shred.unsafe_createInstance(in_class->memorySize, [&](auto in_weak){
        in_class->constructInstance(in_weak);
    });

    int index = 0;
    {
        for (const auto& childIt: children)
            if (childIt->id.name == in_name)
                index = std::max(index, childIt->id.duplicationIndex + 1);
    }
    shred->id = {{},std::move(in_name),index};

    auto shredWeak = shred.getWeak();
    children.push_back(std::move(shred));
    in_beforePropagateRegistration(shredWeak);

    return shredWeak;
}

void Shred::postLoad()
{
    QObj::postLoad();
    for(auto& childIt : children)
    {
        childIt->directParent = selfPtr.unsafe_cast<Shred>();
    }
}
