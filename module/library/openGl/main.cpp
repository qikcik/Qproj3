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

GEN_QSTRUCT(Vec3)
{
    GEN_QSTRUCT_BODY(Vec3)
public:
    FIELDS_BEGIN(serializable{instance})
        float x {0};
        float y {0};
        float z {0};
    FIELDS_END()
};

GEN_QSTRUCT_STATIC_DEF(Vec3,{
    GEN_QSTRUCT_FIELD_ENTRY(Vec3,x),
    GEN_QSTRUCT_FIELD_ENTRY(Vec3,y),
    GEN_QSTRUCT_FIELD_ENTRY(Vec3,z),
});

GEN_QSTRUCT(Cube)
{
    GEN_QSTRUCT_BODY(Cube)
public:
    FIELDS_BEGIN(serializable{instance})
    Vec3 pos = {{},0,0,-8};
    Vec3 rot = {};
    Vec3 rotChange = {};
    FIELDS_END()
};

GEN_QSTRUCT_STATIC_DEF(Cube,{
    GEN_QSTRUCT_FIELD_ENTRY(Cube,pos),
    GEN_QSTRUCT_FIELD_ENTRY(Cube,rot),
    GEN_QSTRUCT_FIELD_ENTRY(Cube,rotChange),
});

GEN_QSTRUCT(App)
{
    GEN_QSTRUCT_BODY(App)
public:
    FIELDS_BEGIN(serializable{instance})
    OwnerPtr<Shred> root {};

    DynamicArray<Cube> arr {};
    FIELDS_END()
};

GEN_QSTRUCT_STATIC_DEF(App,{
    GEN_QSTRUCT_FIELD_ENTRY(App,root),
    GEN_QSTRUCT_FIELD_ENTRY(App,arr),
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
    rti.registerQObjDef(StringPropertyView::staticDef.getWeak()); StringPropertyView::staticDef->initializeQObjDef();
    rti.registerQObjDef(QStructPropertyView::staticDef.getWeak()); QStructPropertyView::staticDef->initializeQObjDef();
    rti.registerQObjDef(DynamicArrayPropertyView::staticDef.getWeak()); DynamicArrayPropertyView::staticDef->initializeQObjDef();
    Converter conv(rti);

    App app {};
    app.arr.reserve(20);
    app.root = Shred::ConstructRoot(Shred::staticDef.getWeak());
    auto controller = app.root->appendChildren<NativeController>("window!");
    auto win = controller->appendChildren<NativeWindow>("winApi");
    auto strct = win->appendChildren<QStructPropertyView>("struct");
    strct->setPosition({{},5,5});
    strct->set("App",&app,App::staticDef.getWeak());





    auto gl = controller->appendChildren<OpenGlWindow>("gl");
    GLuint texture[3];
    float z {-5};
    bool light {false};
    GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };
    GLuint  filter {2};

    bool lp {false};
    bool fp {false};

    gl->onInit = [&]()
    {
        glGenTextures(3, &texture[0]);

        glBindTexture(GL_TEXTURE_2D, texture[0]);
        loadTexture("asset/texture.png");
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, texture[1]);
        loadTexture("asset/texture.png");
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, texture[2]);
        loadTexture("asset/texture.png",true);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);

        glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
        glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHTING);
        return true;
    };

    gl->onUpdate = [&](float delta)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(auto& cubeIt : app.arr)
        {
            cubeIt.rot.x += cubeIt.rotChange.x;
            cubeIt.rot.y += cubeIt.rotChange.y;
            cubeIt.rot.z += cubeIt.rotChange.z;

            glLoadIdentity();
            glTranslatef(cubeIt.pos.x, cubeIt.pos.y, cubeIt.pos.z);

            glRotatef(cubeIt.rot.x, 1.0f, 0.0f, 0.0f);
            glRotatef(cubeIt.rot.y, 0.0f, 1.0f, 0.0f);
            glRotatef(cubeIt.rot.z, 0.0f, 0.0f, 1.0f);

            glColor3f(1.0f, 1.0f, 1.0f);
            glBindTexture(GL_TEXTURE_2D, texture[filter]);
            glBegin(GL_QUADS);
            // Front Face
            glNormal3f(0.0f, 0.0f, 1.0f);                  // Normal Pointing Towards Viewer
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f, 1.0f);  // Point 1 (Front)
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(1.0f, -1.0f, 1.0f);  // Point 2 (Front)
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(1.0f, 1.0f, 1.0f);  // Point 3 (Front)
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-1.0f, 1.0f, 1.0f);  // Point 4 (Front)
            // Back Face
            glNormal3f(0.0f, 0.0f, -1.0f);                  // Normal Pointing Away From Viewer
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Back)
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(-1.0f, 1.0f, -1.0f);  // Point 2 (Back)
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(1.0f, 1.0f, -1.0f);  // Point 3 (Back)
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(1.0f, -1.0f, -1.0f);  // Point 4 (Back)
            // Top Face
            glNormal3f(0.0f, 1.0f, 0.0f);                  // Normal Pointing Up
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-1.0f, 1.0f, -1.0f);  // Point 1 (Top)
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-1.0f, 1.0f, 1.0f);  // Point 2 (Top)
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(1.0f, 1.0f, 1.0f);  // Point 3 (Top)
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(1.0f, 1.0f, -1.0f);  // Point 4 (Top)
            // Bottom Face
            glNormal3f(0.0f, -1.0f, 0.0f);                  // Normal Pointing Down
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Bottom)
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(1.0f, -1.0f, -1.0f);  // Point 2 (Bottom)
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(1.0f, -1.0f, 1.0f);  // Point 3 (Bottom)
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f, 1.0f);  // Point 4 (Bottom)
            // Right face
            glNormal3f(1.0f, 0.0f, 0.0f);                  // Normal Pointing Right
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(1.0f, -1.0f, -1.0f);  // Point 1 (Right)
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(1.0f, 1.0f, -1.0f);  // Point 2 (Right)
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(1.0f, 1.0f, 1.0f);  // Point 3 (Right)
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(1.0f, -1.0f, 1.0f);  // Point 4 (Right)
            // Left Face
            glNormal3f(-1.0f, 0.0f, 0.0f);                  // Normal Pointing Left
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Left)
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f, 1.0f);  // Point 2 (Left)
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(-1.0f, 1.0f, 1.0f);  // Point 3 (Left)
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-1.0f, 1.0f, -1.0f);  // Point 4 (Left)
            glEnd();
        }
    };



    controller->loop(hInstance,nCmdShow);


    return 0;
}