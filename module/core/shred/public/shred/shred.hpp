#pragma once

#include "typeSystem/qStructDef.hpp"
#include "typeSystem/qObjDef.hpp"
#include "typeSystem/qObj.hpp"
#include "typeSystem/fieldType/all.hpp"
#include "typeSystem/macros.hpp"
#include "typeSystem/structMacros.hpp"

#include <stack>

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
    template<std::derived_from<class Shred> T>
    WeakPtr<T> appendChildren(std::string in_name) {
        auto shred = appendChildren(T::staticDef.getWeak(),std::move(in_name));
        return std::move(shred.template unsafe_cast<T>());
    };

    WeakPtr<Shred> appendChildren(WeakPtr<QObjDef> in_class, std::string in_name,const std::function<void(WeakPtr<Shred>)>& in_beforePropagateRegistration = [](auto ptr){});
    std::string getUniqueName() {return id.getUniqueName(); }
    WeakPtr<Shred> getDirectParent() {return directParent; }


    template<class T>
    DynamicArray<WeakPtr<T>> getChildsOfClass_singleDeep()
    {
        DynamicArray<WeakPtr<T>> result;
        std::stack<WeakPtr<Shred>> stack;
        stack.push(selfPtr.unsafe_cast<Shred>());
        while(!stack.empty())
        {
            auto& el = stack.top(); stack.pop();
            for(auto& childIt : el->children)
            {
                if( childIt->getObjDef()->isBaseOrSame( T::staticDef.getWeak() ))
                    result.push_back(childIt.getWeak().unsafe_cast<T>());
                else
                    stack.push(childIt.getWeak());
            }
        }
        return result;
    }

    template<class T>
    WeakPtr<T> getDirectChildOfClass()
    {
        for( auto& childIt : children)
        {
            if( childIt->getObjDef()->isBaseOrSame( T::staticDef.getWeak() ))
                return childIt.getWeak().unsafe_cast<T>();
        }
        return {};
    }

public:
    void postLoad() override;


protected:
    ShredLocalIdentifier id;
    WeakPtr<Shred> directParent {};
    DynamicArray<OwnerPtr<Shred>> children {};
};