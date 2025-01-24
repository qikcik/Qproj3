#include "propertyViewer/floatPropertyView.hpp"
#include "gui/NativeEditbox.hpp"
#include "gui/nativeLabel.hpp"

BEGIN_GEN_QOBJ_STATIC_DEF(FloatPropertyView,Shred)
selfPtr->fields = std::move(DynamicArray<OwnerPtr<Field>>
{

});
END_GEN_QOBJ_STATIC_DEF()

float toString(const std::string& in_str)
{
    try {
        return std::stof(in_str);
    }
    catch(std::exception ex) {
        return 0.0;
    }
}


void FloatPropertyView::set(void* in_ptr, std::string name, WeakPtr<FloatField> in_type)
{
    ptr = static_cast<float*>(in_ptr);

    label = appendChildren<NativeLabel>("label");
    label->setText(name);

    edit = appendChildren<NativeEditbox>("edit");
    edit->setText(std::to_string(*ptr));
    edit->setType(NativeEditbox::Type::Float);

    auto lambdaPtr = ptr;
    edit->onChange = [lambdaPtr](auto in_edit){
        *lambdaPtr = toString(in_edit->getText());
    };

    setPosition(pos);
}

Vec2 FloatPropertyView::getSize()
{
    return {{},200,20};
}

void FloatPropertyView::setPosition(Vec2 in_pos)
{
    pos = in_pos;
    if(label) label->setScreenRect({in_pos.x,in_pos.y,100,20});
    if(edit) edit->setScreenRect({in_pos.x+100,in_pos.y,100,20});
}

void FloatPropertyView::handleTick(float delta)
{
    DBG_ENSURE_OR_RETURN(edit,);
    auto val = toString(edit->getText());
    if(*ptr != val)
    {
        edit->setText(std::to_string(*ptr));
    }
}
