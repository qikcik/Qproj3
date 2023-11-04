#pragma once
#include "typeSystem/fieldType.hpp"

class QObjDef;
class OwnerObjPtrField : public FieldType
{
public:
    explicit OwnerObjPtrField(const WeakPtr<QObjDef>& in_type) : type(in_type) {}
    ~OwnerObjPtrField() override = default;

    const WeakPtr<QObjDef> type; // TODO: useless on the time of generation to itself, in not initiated yet

    std::string getTypeDesc() const override;

    size_t getMemorySize() const override {
        return sizeof(OwnerPtr<void>);
    };

    json::innerType toJson(void* in_addr) const  override;
    void fromJson(void* in_addr,const json::innerType& in_json,LoadingContext& in_context) const override;
};

template<typename T>
concept TOwnerPtr = requires(T a) {
    requires std::same_as<T, OwnerPtr<typename T::element_type>>;
};

template<TOwnerPtr T>
OwnerPtr<FieldType> getFieldType();