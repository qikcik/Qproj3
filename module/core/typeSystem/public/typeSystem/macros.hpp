#define GEN_QOBJ(NAME,BASE) class NAME : public BASE
#define GEN_IMPL_INTERFACE(INTERFACE) , public INTERFACE
#define GEN_QOBJ_BODY(NAME,BASE) public: static const OwnerPtr<QObjDef> staticDef; private:

#define GEN_INTERFACE(NAME) class NAME

#define GEN_QOBJ_DEF_CONSTRUCTOR(NAME,BASE) \
public:                                                 \
explicit NAME(WeakPtr<QObj> in_self,WeakPtr<QObjDef> in_def) : BASE(std::move(in_self),std::move(in_def)) {};  \
private:

#define GEN_QOBJ_DEF_CONSTRUCTOR_AND_DESTRUCTOR(NAME,BASE) \
public:                                                 \
explicit NAME(WeakPtr<QObj> in_self,WeakPtr<QObjDef> in_def) : BASE(std::move(in_self),std::move(in_def)) {};  \
~NAME() override = default;                      \
private:


#define BEGIN_GEN_QOBJ_STATIC_DEF(NAME,BASE) \
const OwnerPtr<QObjDef> NAME::staticDef = std::move(OwnerPtr<QObjDef>::CreateWithInstance( \
        #NAME, \
        sizeof(NAME),                             \
        [](WeakPtr<QObj> in_addr){ new ((void*)in_addr.getPtr()) NAME(in_addr,NAME::staticDef.getWeak()); }, \
        [](WeakPtr<QObj> in_addr){ reinterpret_cast<NAME*>((void*)in_addr.getPtr())->~NAME(); }, \
        [](){ auto selfPtr = NAME::staticDef.unsafe_getPtr(); selfPtr->baseObjDef = std::move(BASE::staticDef.getWeak());

#define END_GEN_QOBJ_STATIC_DEF() }));


#define GEN_QOBJ_DEF_FIELD_ENTRY(type,name) OwnerPtr<Field>::CreateWithInstance(#name, offsetof(type, name), std::move(getFieldType<typeof(type::name)>()))
#define FIELD(args...)
#define FIELDS_BEGIN(args...)
#define FIELDS_END()