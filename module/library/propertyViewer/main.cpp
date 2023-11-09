#include <windows.h>
#include <fstream>
#include <sstream>
#include "gui/nativeWindow.hpp"
#include "gui/nativeLabel.hpp"
#include "typeSystem/converter.hpp"
#include "json/parser.hpp"
#include "gui/openGlWindow.hpp"
#include "gui/nativeController.hpp"
#include <gl/gl.h>
#include <GL/glu.h>
#include "stb_image/stb_image.h"
#include "gui/NativeEditbox.hpp"
#include "propertyViewer/stringPropertyView.hpp"
#include "propertyViewer/qstructPropertyView.hpp"
#include "gui/nativeGroupbox.hpp"
#include "propertyViewer/dynamicArrayPropertyView.hpp"
#include "gui/nativeButton.hpp"


GEN_QSTRUCT(App)
{
    GEN_QSTRUCT_BODY(App)
public:
    FIELDS_BEGIN(serializable{instance})
        OwnerPtr<Shred> root {};
        std::string test {"value"};
        float foo {23.2};
        DynamicArray<float> Arr {2,3,4};
        std::string Arr2 {};
    FIELDS_END()
};

GEN_QSTRUCT_STATIC_DEF(App,{
    GEN_QSTRUCT_FIELD_ENTRY(App,root),
    GEN_QSTRUCT_FIELD_ENTRY(App,test),
    GEN_QSTRUCT_FIELD_ENTRY(App,foo),
    GEN_QSTRUCT_FIELD_ENTRY(App,Arr),
    GEN_QSTRUCT_FIELD_ENTRY(App,Arr2),
});


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    RuntimeTypeInformation rti;
    rti.registerQObjDef(Shred::staticDef.getWeak()); Shred::staticDef->initializeQObjDef();
    rti.registerQObjDef(NativeWindow::staticDef.getWeak()); NativeWindow::staticDef->initializeQObjDef();
    rti.registerQObjDef(NativeWidget::staticDef.getWeak()); NativeWidget::staticDef->initializeQObjDef();
    rti.registerQObjDef(NativeLabel::staticDef.getWeak()); NativeLabel::staticDef->initializeQObjDef();
    rti.registerQObjDef(OpenGlWindow::staticDef.getWeak()); OpenGlWindow::staticDef->initializeQObjDef();
    rti.registerQObjDef(NativeController::staticDef.getWeak()); NativeController::staticDef->initializeQObjDef();
    rti.registerQObjDef(NativeEditbox::staticDef.getWeak()); NativeEditbox::staticDef->initializeQObjDef();
    rti.registerQObjDef(NativeGroupbox::staticDef.getWeak()); NativeGroupbox::staticDef->initializeQObjDef();
    rti.registerQObjDef(NativeButton::staticDef.getWeak()); NativeButton::staticDef->initializeQObjDef();
    rti.registerQObjDef(StringPropertyView::staticDef.getWeak()); StringPropertyView::staticDef->initializeQObjDef();
    rti.registerQObjDef(QStructPropertyView::staticDef.getWeak()); QStructPropertyView::staticDef->initializeQObjDef();
    rti.registerQObjDef(DynamicArrayPropertyView::staticDef.getWeak()); DynamicArrayPropertyView::staticDef->initializeQObjDef();

    Converter conv(rti);

    App app {};
    app.root = Shred::ConstructRoot(Shred::staticDef.getWeak());
    auto controller = app.root->appendChildren<NativeController>("window!");
    auto win = controller->appendChildren<NativeWindow>("winApi");
    auto strct = win->appendChildren<QStructPropertyView>("struct");
    strct->set(&app,App::staticDef.getWeak());


    controller->loop(hInstance,nCmdShow);


    return 0;
}