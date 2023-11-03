#pragma once
#include "template/container/dynamicArray.hpp"
#include "typeSystem/fieldType.hpp"

class DynamicArrayField : public FieldType
{
public:
    explicit DynamicArrayField(const OwnerPtr<FieldType>&& in_type) : valueType(std::move(in_type)) {}
    ~DynamicArrayField() override = default;

    const OwnerPtr<FieldType> valueType;

    std::string getTypeDesc() const override {
        return "DynamicArray<"+valueType->getTypeDesc()+">";
    }

    size_t getMemorySize() const override {
        return sizeof(DynamicArray<uint8_t>);
    };

    json::innerType toJson(void* in_addr) const  override;
    void fromJson(void* in_addr,const json::innerType& in_json,LoadingContext& in_context) const override;
};

template<typename T>
concept TDynamicArrayConcept = requires(T a) {
    requires std::same_as<T, DynamicArray<typename T::element_type>>;
};

template<TDynamicArrayConcept T>
OwnerPtr<FieldType> getFieldType();