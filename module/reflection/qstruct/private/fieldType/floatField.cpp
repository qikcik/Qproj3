
#include "qstruct/fieldType/floatField.hpp"


template<> OwnerPtr<FieldType> getFieldType<float>() { return OwnerPtr<FloatField>::CreateWithInstance(); };