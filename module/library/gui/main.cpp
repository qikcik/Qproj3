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
    Converter conv(rti);

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

    app.root = Shred::ConstructRoot(Shred::staticDef.getWeak());
    auto controller = app.root->appendChildren<NativeController>("window!");

    auto gl = controller->appendChildren<OpenGlWindow>("gl");
    GLuint texture[3];
    float rotX {0};
    float rotY {0};
    float z {-5};
    bool light {false};
    GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };
    GLuint  filter {0};

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
        return true;
    };

    gl->onUpdate = [&](float delta)
    {
        if(gl->isKeyPressed(VK_UP)) rotX-= 2;
        if(gl->isKeyPressed(VK_DOWN)) rotX+= 2;
        if(gl->isKeyPressed(VK_LEFT)) rotY-= 2;
        if(gl->isKeyPressed(VK_RIGHT)) rotY+= 2;

        if (gl->isKeyPressed('L') && !lp)
        {
            lp=true;
            light=!light;

            if (!light)
                glDisable(GL_LIGHTING);
            else
                glEnable(GL_LIGHTING);
        }
        if(!gl->isKeyPressed('L'))
            lp = false;

        if (gl->isKeyPressed('F') && !fp)
        {
            fp=true;
            filter+=1;
            if(filter>2) filter = 0;
        }
        if(!gl->isKeyPressed('F'))
            fp = false;

        if (gl->isKeyPressed(VK_PRIOR))
            z+=0.05f;
        if (gl->isKeyPressed(VK_NEXT))
            z-=0.05f;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glTranslatef(0.0,0.0f,z);
        glRotatef(rotX,1.0f,0.0f,0.0f);
        glRotatef(rotY,0.0f,1.0f,0.0f);
        glColor3f(1.0f,1.0f,1.0f);
        glBindTexture(GL_TEXTURE_2D, texture[filter]);
        glBegin(GL_QUADS);
            // Front Face
            glNormal3f( 0.0f, 0.0f, 1.0f);                  // Normal Pointing Towards Viewer
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 1 (Front)
            glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 2 (Front)
            glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Front)
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 4 (Front)
            // Back Face
            glNormal3f( 0.0f, 0.0f,-1.0f);                  // Normal Pointing Away From Viewer
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Back)
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 2 (Back)
            glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 3 (Back)
            glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 4 (Back)
            // Top Face
            glNormal3f( 0.0f, 1.0f, 0.0f);                  // Normal Pointing Up
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 1 (Top)
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 2 (Top)
            glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Top)
            glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 4 (Top)
            // Bottom Face
            glNormal3f( 0.0f,-1.0f, 0.0f);                  // Normal Pointing Down
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Bottom)
            glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 2 (Bottom)
            glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 3 (Bottom)
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 4 (Bottom)
            // Right face
            glNormal3f( 1.0f, 0.0f, 0.0f);                  // Normal Pointing Right
            glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 1 (Right)
            glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 2 (Right)
            glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Right)
            glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 4 (Right)
            // Left Face
            glNormal3f(-1.0f, 0.0f, 0.0f);                  // Normal Pointing Left
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Left)
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 2 (Left)
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 3 (Left)
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 4 (Left)
        glEnd();
    };













    auto win2 = controller->appendChildren<NativeWindow>("winApi");
    auto lbl = win2->appendChildren<NativeLabel>("label");
    lbl->setText("test");
    lbl->setScreenRect({{},20,20,100,100});

    auto edit = win2->appendChildren<NativeEditbox>("edit");
    edit->setText("test");
    edit->setScreenRect({{},200,200,100,20});

    controller->loop(hInstance,nCmdShow);

    std::ofstream outFile {};
    outFile.open ( "layout.json");
    auto json = conv.qstructToJson(&app, App::staticDef.getWeak());
    std::string source = json.stringify();
    outFile << source;

    return 0;
}