#define GEN_QOBJ(NAME,BASE) class NAME : public BASE
#define GEN_QOBJ_BODY(NAME,BASE) public: static const OwnerPtr<QObjDef> staticDef; private:

#define GEN_QOBJ_DEF_CONSTRUCTOR_AND_DESTRUCTOR(NAME,BASE) \
public:                                                 \
explicit NAME(WeakPtr<QObj> in_self,WeakPtr<QObjDef> in_def) : BASE(std::move(in_self),std::move(in_def)) {};  \
~NAME() override = default;                      \
private:


#define BEGIN_GEN_QOBJ_STATIC_DEF(NAME,BASE) \
const OwnerPtr<QObjDef> NAME::staticDef = std::move(OwnerPtr<QObjDef>::CreateWithInstance( \
        BASE::staticDef.getWeak(),                                          \
        #NAME, \
        sizeof(NAME),                             \
        [](WeakPtr<QObj> in_addr){ new (in_addr.getPtr()) NAME(in_addr,QObj::staticDef.getWeak()); }, \
        [](WeakPtr<QObj> in_addr){ reinterpret_cast<NAME*>(in_addr.getPtr())->~NAME(); },

#define END_GEN_QOBJ_STATIC_DEF() ));


#define GEN_QOBJ_DEF_FIELD_ENTRY(type,name) OwnerPtr<Field>::CreateWithInstance(#name, offsetof(type, name), std::move(getFieldType<typeof(type::name)>()))