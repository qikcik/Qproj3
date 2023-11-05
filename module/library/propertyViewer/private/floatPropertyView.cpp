#include "propertyViewer/floatPropertyView.hpp"
#include "gui/NativeEditbox.hpp"
#include "gui/nativeLabel.hpp"

BEGIN_GEN_QOBJ_STATIC_DEF(FloatPropertyView,Shred)
selfPtr->fields = std::move(DynamicArray<OwnerPtr<Field>>
{

});
END_GEN_QOBJ_STATIC_DEF()

void FloatPropertyView::set(void* in_ptr, std::string name, WeakPtr<FloatField> in_type)
{
    auto* ptr = reinterpret_cast<float*>(in_ptr);

    label = appendChildren<NativeLabel>("label");
    label->setText(name);
    label->setScreenRect({{},0,0,100,20});


    edit = appendChildren<NativeEditbox>("edit");
    edit->setText(std::to_string(*ptr));
    edit->setType(NativeEditbox::Type::Float);
    edit->setScreenRect({{},100,0,100,20});

    edit->onChange = [ptr](auto in_edit){
        try {
            float val = std::stof(in_edit->getText());
            *ptr = val;
        }
        catch(std::exception ex) {
            *ptr = 0.f;
        }
    };
}

Vec2 FloatPropertyView::getSize()
{
    return {{},200,20};
}

void FloatPropertyView::setPosition(Vec2 in_pos)
{
    label->setScreenRect({{},in_pos.x,in_pos.y,100,20});
    edit->setScreenRect({{},in_pos.x+100,in_pos.y,100,20});
}
