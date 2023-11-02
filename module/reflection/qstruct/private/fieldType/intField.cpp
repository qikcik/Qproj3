#include "qstruct/fieldType/intField.hpp"

template<> OwnerPtr<FieldType> getFieldType<int>() { return OwnerPtr<IntField>::CreateWithInstance(); };
