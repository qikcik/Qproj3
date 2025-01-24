#pragma once

#define GEN_QSTRUCT(NAME) struct NAME : public QStruct
#define GEN_QSTRUCT_BODY(NAME) static const OwnerPtr<QStructDef> staticDef;

#define GEN_QSTRUCT_STATIC_DEF(type,entries...) \
const OwnerPtr<QStructDef> type::staticDef = std::move(OwnerPtr<QStructDef>::CreateWithInstance( \
        #type, \
        sizeof(type), \
        [](QStruct* addr){ new (addr) type(); },                                 \
        [](QStruct* addr){ reinterpret_cast<type*>(addr)->~type(); },\
        [](QStruct* from,QStruct* to){ new (reinterpret_cast<type*>(to)) type(std::move(  *reinterpret_cast<type*>(from)  )); },\
        DynamicArray<OwnerPtr<Field>> entries \
));

#define GEN_QSTRUCT_FIELD_ENTRY(type,name) OwnerPtr<Field>::CreateWithInstance(#name, offsetof(type, name), std::move(getFieldType<typeof(type::name)>()))
