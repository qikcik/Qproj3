#include "gui/openGlWindow.hpp"
#include <gl/gl.h>
#include <gl/glu.h>

BEGIN_GEN_QOBJ_STATIC_DEF(OpenGlWindow,NativeWindow)
selfPtr->fields = std::move(DynamicArray<OwnerPtr<Field>>
{

});
END_GEN_QOBJ_STATIC_DEF()

OpenGlWindow::~OpenGlWindow()
{
    if (hRC)
    {
        wglMakeCurrent(nullptr,nullptr);
        wglDeleteContext(hRC);
    }

    if (hDC && !ReleaseDC(hwnd,hDC))
        ReleaseDC(hwnd,hDC);
}

bool OpenGlWindow::postInitializeImpl()
{
    NativeWindow::postInitializeImpl();
    unsigned int PixelFormat;

    static  PIXELFORMATDESCRIPTOR pfd= // pfd Tells Windows How We Want Things To Be
    {
        sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
        1, // Version Number
        PFD_DRAW_TO_WINDOW | // Format Must Support Window
        PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
        PFD_DOUBLEBUFFER, // Must Support Double Buffering
        PFD_TYPE_RGBA, // Request An RGBA Format
        static_cast<BYTE>(16), // Select Our Color Depth
        0, 0, 0, 0, 0, 0, // Color Bits Ignored
        0, // No Alpha Buffer
        0, // Shift Bit Ignored
        0, // No Accumulation Buffer
        0, 0, 0, 0, // Accumulation Bits Ignored
        16, // 16Bit Z-Buffer (Depth Buffer)
        0, // No Stencil Buffer
        0, // No Auxiliary Buffer
        PFD_MAIN_PLANE, // Main Drawing Layer
        0, // Reserved
        0, 0, 0 // Layer Masks Ignored
    };

    if (!(hDC=GetDC(hwnd)))
        return false;

    if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))
        return false;

    if(!SetPixelFormat(hDC,PixelFormat,&pfd))
        return false;

    if (!(hRC=wglCreateContext(hDC)))
        return false;

    if(!wglMakeCurrent(hDC,hRC))
        return false;

    InitGL();
    onInit();
    return true;
}


void OpenGlWindow::InitGL()
{
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void OpenGlWindow::onResizeImpl(int in_x, int in_y)
{
    NativeWindow::onResizeImpl(in_x, in_y);
    if (in_y==0) in_y=1;

    glViewport(0, 0, in_x, in_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f,(GLfloat)in_x/(GLfloat)in_y,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OpenGlWindow::handleTick(float in_delta)
{
    if(!focused) return;

    onUpdate(in_delta);

    SwapBuffers(hDC);
}