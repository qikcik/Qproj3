#include "propertyViewer/stringPropertyView.hpp"
#include "gui/NativeEditbox.hpp"
#include "gui/nativeLabel.hpp"

BEGIN_GEN_QOBJ_STATIC_DEF(StringPropertyView,Shred)
selfPtr->fields = std::move(DynamicArray<OwnerPtr<Field>>
{

});
END_GEN_QOBJ_STATIC_DEF()

void StringPropertyView::set(void* in_ptr, std::string name, WeakPtr<StdStringField> in_type)
{
    auto* ptr = reinterpret_cast<std::string*>(in_ptr);

    label = appendChildren<NativeLabel>("label");
    label->setText(name);
    label->setScreenRect({{},0,0,100,20});


    edit = appendChildren<NativeEditbox>("edit");
    edit->setText(*ptr);
    edit->setScreenRect({{},100,0,100,20});

    edit->onChange = [ptr](auto in_edit){
        *ptr = in_edit->getText();
    };
}

Vec2 StringPropertyView::getSize()
{
    return {{},200,20};
}

void StringPropertyView::setPosition(Vec2 in_pos)
{
    label->setScreenRect({{},in_pos.x,in_pos.y,100,20});
    edit->setScreenRect({{},in_pos.x+100,in_pos.y,100,20});
}
