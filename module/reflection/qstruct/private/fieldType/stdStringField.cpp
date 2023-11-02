#include "qstruct/fieldType/stdStringField.hpp"

template<> OwnerPtr<FieldType> getFieldType<std::string>() { return OwnerPtr<StdStringField>::CreateWithInstance(); };