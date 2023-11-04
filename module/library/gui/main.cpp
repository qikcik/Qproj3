#include <windows.h>
#include <fstream>
#include <sstream>
#include "gui/nativeWindow.hpp"
#include "gui/nativeLabel.hpp"
#include "typeSystem/converter.hpp"
#include "json/parser.hpp"
#include "gui/openGlWindow.hpp"
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
    rti.registerQObjDef(Shred::staticDef.getWeak()); Shred::staticDef->initializeQObjDef();
    rti.registerQObjDef(NativeWindow::staticDef.getWeak()); NativeWindow::staticDef->initializeQObjDef();
    rti.registerQObjDef(NativeWidget::staticDef.getWeak()); NativeWidget::staticDef->initializeQObjDef();
    rti.registerQObjDef(NativeLabel::staticDef.getWeak()); NativeLabel::staticDef->initializeQObjDef();
    rti.registerQObjDef(OpenGlWindow::staticDef.getWeak()); OpenGlWindow::staticDef->initializeQObjDef();
    Converter conv(rti);
    auto dbg = OpenGlWindow::staticDef.unsafe_getPtr();
    App app {};

    std::ifstream file("layout.json");
//    if(file.good())
//    {
//        std::stringstream stream{};
//        stream << file.rdbuf();
//        auto json = std::get<json::Object>(json::Parser().parse(stream.str()));
//        conv.jsonToQStruct(json, App::staticDef.getWeak(), &app);
//        file.close();
//    }
//    else
    {
        app.root = Shred::ConstructRoot(Shred::staticDef.getWeak());
        auto win = app.root->appendChildren<OpenGlWindow>("window!");
        auto win2 = app.root->appendChildren<NativeWindow>("window!");
        auto lbl = win2->appendChildren<NativeLabel>("label");
        lbl->setText("test");
        lbl->setScreenRect({{},20,20,100,100});
    }


    for(auto& winIt : app.root->getChildsOfClass_singleDeep<NativeWindow>())
        winIt->initializeNative(hInstance,nCmdShow);


    MSG msg;
    bool done = false;

    while(!done) // Loop That Runs Until done=TRUE
    {
        if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))           // Is There A Message Waiting?
        {
            if (msg.message==WM_QUIT)
            {
                done=true;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else {
            for( auto& winIt : app.root->getChildsOfClass_singleDeep<NativeWindow>())
                winIt->onUpdate(1/60.f); // TODO change to real delta
        }
    }


    std::ofstream outFile {};
    outFile.open ( "layout.json");
    auto json = conv.qstructToJson(&app, App::staticDef.getWeak());
    std::string source = json.stringify();
    outFile << source;

    return 0;
}