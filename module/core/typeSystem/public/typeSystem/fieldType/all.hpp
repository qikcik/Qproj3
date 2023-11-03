#pragma once
#include "typeSystem/fieldType/dynamicArrayField.hpp"
#include "typeSystem/fieldType/floatField.hpp"
#include "typeSystem/fieldType/intField.hpp"
#include "typeSystem/fieldType/qStructField.hpp"
#include "typeSystem/fieldType/stdStringField.hpp"
#include "typeSystem/fieldType/stdStringField.hpp"
#include "typeSystem/fieldType/ownerObjPtrField.hpp"


template<TDynamicArrayConcept T>
OwnerPtr<FieldType> getFieldType() {
    using  element_type = typename T::element_type;
    return OwnerPtr<DynamicArrayField>::CreateWithInstance( std::move(getFieldType<element_type>()) );
}

template<std::derived_from<class QStruct> T>
OwnerPtr<FieldType> getFieldType() {
    return OwnerPtr<QStructField>::CreateWithInstance( T::staticDef.getWeak() );
}

template<TOwnerPtr T>
OwnerPtr<FieldType> getFieldType() {
    return OwnerPtr<OwnerObjPtrField>::CreateWithInstance( T::element_type::staticDef.getWeak() );
}

