#include <windows.h>
#include <fstream>
#include <sstream>
#include "gui/nativeWindow.hpp"
#include "gui/nativeLabel.hpp"
#include "gui/nativeGroupbox.hpp"
#include "typeSystem/converter.hpp"
#include "json/parser.hpp"
#include "gui/openGlWindow.hpp"
#include "gui/nativeController.hpp"
#include <gl/gl.h>
#include <GL/glu.h>
#include "stb_image/stb_image.h"
#include "gui/NativeEditbox.hpp"
#include "gui/nativeButton.hpp"


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

bool loadTexture(const std::string& in_path,bool mipmap = false)
{
    int w;
    int h;
    int comp;
    unsigned char* img = stbi_load(in_path.c_str(), &w, &h, &comp, STBI_rgb_alpha );
    if(img == nullptr)
        return false;

    if(mipmap)
    {
        if (comp == 3)
            gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, img);
        else if (comp == 4)
            gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGBA, GL_UNSIGNED_BYTE, img);
    }
    else
    {
        if (comp == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
        else if (comp == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    }
    stbi_image_free(img);
    return true;
}

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
        auto controller = app.root->appendChildren<NativeController>("window!");


        auto win2 = controller->appendChildren<NativeWindow>("winApi");
        auto lbl = win2->appendChildren<NativeLabel>("label");
        lbl->setText("test");
        lbl->setScreenRect({20,20,100,100});

        auto edit = win2->appendChildren<NativeEditbox>("edit");
        edit->setText("test");
        edit->setScreenRect({200,200,100,20});

        auto gr = win2->appendChildren<NativeButton>("button");
        gr->setText("grrrrr");
        gr->setScreenRect({300,300,100,20});
    }

    auto controller = app.root->getDirectChildOfClass<NativeController>();
    controller->loop(hInstance,nCmdShow);

    std::ofstream outFile {};
    outFile.open ( "layout.json");
    auto json = conv.qstructToJson(&app, App::staticDef.getWeak());
    std::string source = json.stringify();
    outFile << source;

    return 0;
}