#pragma once
#include "qstruct/fieldType/dynamicArrayField.hpp"
#include "qstruct/fieldType/floatField.hpp"
#include "qstruct/fieldType/intField.hpp"
#include "qstruct/fieldType/qStructField.hpp"
#include "qstruct/fieldType/stdStringField.hpp"
#include "qstruct/fieldType/stdStringField.hpp"


template<TDynamicArrayConcept T>
OwnerPtr<FieldType> getFieldType() {
    using  element_type = typename T::element_type;
    return OwnerPtr<DynamicArrayField>::CreateWithInstance( std::move(getFieldType<element_type>()) );
}

template<std::derived_from<class QStruct> T>
OwnerPtr<FieldType> getFieldType() {
    return OwnerPtr<QStructField>::CreateWithInstance( T::staticDef.getWeak() );
}

