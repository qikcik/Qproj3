#include "typeSystem/fieldType/intField.hpp"

template<> OwnerPtr<FieldType> getFieldType<int>() { return OwnerPtr<IntField>::CreateWithInstance(); };
