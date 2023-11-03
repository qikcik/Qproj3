#pragma once

#define GEN_QSTRUCT(NAME) struct NAME : public QStruct
#define GEN_QSTRUCT_BODY(NAME) static const OwnerPtr<QStructDef> staticDef;

#define GEN_QSTRUCT_STATIC_DEF(type,entries...) \
const OwnerPtr<QStructDef> type::staticDef = std::move(OwnerPtr<QStructDef>::CreateWithInstance( \
        #type, \
        sizeof(type), \
        [](QStruct* addr){ new (addr) type(); },                                 \
        [](QStruct* addr){ reinterpret_cast<type*>(addr)->~type(); },\
        DynamicArray<OwnerPtr<Field>> entries \
));

#define GEN_QSTRUCT_FIELD_ENTRY(type,name) OwnerPtr<Field>::CreateWithInstance(#name, offsetof(type, name), std::move(getFieldType<typeof(type::name)>()))
