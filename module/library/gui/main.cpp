#include <windows.h>
#include <fstream>
#include <sstream>
#include "gui/nativeWindow.hpp"
#include "gui/nativeLabel.hpp"
#include "typeSystem/converter.hpp"
#include "json/parser.hpp"
#include <gl/gl.h>

GEN_QSTRUCT(App)
{
    GEN_QSTRUCT_BODY(App)
public:
    FIELDS_BEGIN(serializable{instance})
        OwnerPtr<Shred> root {};
    FIELDS_END()
};

GEN_QSTRUCT_STATIC_DEF(App,{
    GEN_QSTRUCT_FIELD_ENTRY(App,root),
});

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    RuntimeTypeInformation rti;
    rti.registerQObjDef(Shred::staticDef.getWeak());
    rti.registerQObjDef(NativeWindow::staticDef.getWeak());
    rti.registerQObjDef(NativeWidget::staticDef.getWeak());
    rti.registerQObjDef(NativeLabel::staticDef.getWeak());
    Converter conv(rti);

    App app {};

    std::ifstream file("layout.json");
    if(file.good())
    {
        std::stringstream stream{};
        stream << file.rdbuf();
        auto json = std::get<json::Object>(json::Parser().parse(stream.str()));
        conv.jsonToQStruct(json, App::staticDef.getWeak(), &app);
        file.close();
    }
    else
    {
        app.root = Shred::ConstructRoot(Shred::staticDef.getWeak());
        auto win = app.root->appendChildren<NativeWindow>("window!");
        auto lbl = win->appendChildren<NativeLabel>("label");
        lbl->setText("test");
        lbl->setScreenRect({{},20,20,100,100});
    }

    auto win = app.root->getDirectChildOfClass<NativeWindow>();
    ENSURE_OR_RETURN(win,-1);

    win->initializeNative(hInstance,nCmdShow);
    MSG msg;
    while( GetMessage( &msg, nullptr, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }

    std::ofstream outFile {};
    outFile.open ( "layout.json");
    auto json = conv.qstructToJson(&app, App::staticDef.getWeak());
    std::string source = json.stringify();
    outFile << source;

    return 0;
}