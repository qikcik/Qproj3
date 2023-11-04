#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <string>
#include "stb_image/stb_image.h"


HGLRC hRC=NULL; // Permanent Rendering Context
HDC hDC=NULL; // Private GDI Device Context
HWND hWnd=NULL; // Holds Our Window Handle
HINSTANCE hInstance; // Holds The Instance Of The Application

bool keys[256]; // Array Used For The Keyboard Routine
bool active=TRUE; // Window Active Flag Set To TRUE By Default

GLuint texture[1];

int LoadGLTextures() // Load Bitmaps And Convert To Textures
{
    int w;
    int h;
    int comp;
    unsigned char* img = stbi_load("asset/texture.png", &w, &h, &comp, STBI_rgb_alpha );
    if(img == nullptr)
        return false;

    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    if(comp == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    else if(comp == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(img);
    return true;
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // Declaration For WndProc

GLvoid ReSizeGLScene(GLsizei width, GLsizei height) // Resize And Initialize The GL Window
{
    if (height==0) // Prevent A Divide By Zero By
    {
        height=1; // Making Height Equal One
    }

    glViewport(0, 0, width, height); // Reset The Current Viewport
    glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
    glLoadIdentity(); // Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
    glLoadIdentity(); // Reset The Modelview Matrix
}

int InitGL(GLvoid) // All Setup For OpenGL Goes Here
{
//    if (!LoadGLTextures())                          // Jump To Texture Loading Routine ( NEW )
//    {
//        return FALSE;                           // If Texture Didn't Load Return FALSE ( NEW )
//    }
    glEnable(GL_TEXTURE_2D);                        // Enable Texture Mapping ( NEW )
    glShadeModel(GL_SMOOTH); // Enables Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Black Background
    glClearDepth(1.0f); // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    glDepthFunc(GL_LEQUAL); // The Type Of Depth Test To Do


    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really Nice Perspective Calculations
    return TRUE; // Initialization Went OK
}
GLfloat     rtri {0};                       // Angle For The Triangle ( NEW )
GLfloat     rquad {0};                      // Angle For The Quad     ( NEW )

GLfloat     xrot;                               // X Rotation ( NEW )
GLfloat     yrot;                               // Y Rotation ( NEW )
GLfloat     zrot;                               // Z Rotation ( NEW )

int DrawGLScene(GLvoid) // Here's Where We Do All The Drawing
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer
    glLoadIdentity();                   // Reset The View
    glTranslatef(-1.5f,0.0f,-6.0f);             // Move Left And Into The Screen

    glRotatef(rtri,0.0f,1.0f,0.0f);             // Rotate The Pyramid On It's Y Axis

    glBegin(GL_TRIANGLES);                  // Start Drawing The Pyramid
        glColor3f(1.0f,0.0f,0.0f);          // Red
        glVertex3f( 0.0f, 1.0f, 0.0f);          // Top Of Triangle (Front)
        glColor3f(0.0f,1.0f,0.0f);          // Green
        glVertex3f(-1.0f,-1.0f, 1.0f);          // Left Of Triangle (Front)
        glColor3f(0.0f,0.0f,1.0f);          // Blue
        glVertex3f( 1.0f,-1.0f, 1.0f);          // Right Of Triangle (Front)

        glColor3f(1.0f,0.0f,0.0f);          // Red
        glVertex3f( 0.0f, 1.0f, 0.0f);          // Top Of Triangle (Right)
        glColor3f(0.0f,0.0f,1.0f);          // Blue
        glVertex3f( 1.0f,-1.0f, 1.0f);          // Left Of Triangle (Right)
        glColor3f(0.0f,1.0f,0.0f);          // Green
        glVertex3f( 1.0f,-1.0f, -1.0f);         // Right Of Triangle (Right)

        glColor3f(1.0f,0.0f,0.0f);          // Red
        glVertex3f( 0.0f, 1.0f, 0.0f);          // Top Of Triangle (Back)
        glColor3f(0.0f,1.0f,0.0f);          // Green
        glVertex3f( 1.0f,-1.0f, -1.0f);         // Left Of Triangle (Back)
        glColor3f(0.0f,0.0f,1.0f);          // Blue
        glVertex3f(-1.0f,-1.0f, -1.0f);         // Right Of Triangle (Back)

        glColor3f(1.0f,0.0f,0.0f);          // Red
        glVertex3f( 0.0f, 1.0f, 0.0f);          // Top Of Triangle (Left)
        glColor3f(0.0f,0.0f,1.0f);          // Blue
        glVertex3f(-1.0f,-1.0f,-1.0f);          // Left Of Triangle (Left)
        glColor3f(0.0f,1.0f,0.0f);          // Green
        glVertex3f(-1.0f,-1.0f, 1.0f);          // Right Of Triangle (Left)
    glEnd();                        // Done Drawing The Pyramid

//    glLoadIdentity();
//    glTranslatef(1.5f,0.0f,-7.0f);              // Move Right And Into The Screen
//    glRotatef(rquad,1.0f,1.0f,1.0f);            // Rotate The Cube On X, Y & Z
//    glColor3f(1.0f,1.0f,1.0f);          // allColor
//    glBindTexture(GL_TEXTURE_2D, texture[0]);               // Select Our Texture
//    glBegin(GL_QUADS);
//        // Front Face
//        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
//        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
//        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
//        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
//        // Back Face
//        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
//        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
//        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
//        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
//        // Top Face
//        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
//        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
//        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
//        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
//        // Bottom Face
//        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
//        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
//        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
//        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
//        // Right face
//        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
//        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
//        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
//        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
//        // Left Face
//        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
//        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
//        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
//        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
//    glEnd();

    rtri+=0.2f;                     // Increase The Rotation Variable For The Triangle ( NEW )
    rquad-=0.15f;                       // Decrease The Rotation Variable For The Quad     ( NEW )
    return TRUE; // Everything Went OK
}

GLvoid KillGLWindow(GLvoid) // Properly Kill The Window
{
    if (hRC) // Do We Have A Rendering Context?
    {
        if (!wglMakeCurrent(NULL,NULL)) // Are We Able To Release The DC And RC Contexts?
            MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        if (!wglDeleteContext(hRC)) // Are We Able To Delete The RC?
            MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hRC=NULL;
    }

    if (hDC && !ReleaseDC(hWnd,hDC)) // Are We Able To Release The DC
    {
        MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hDC=NULL; // Set DC To NULL
    }
    if (hWnd && !DestroyWindow(hWnd))  // Are We Able To Destroy The Window?
    {
        MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hWnd=NULL;
    }
    if (!UnregisterClass("OpenGL",hInstance)) // Are We Able To Unregister Class
    {
        MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hInstance=NULL;
    }
}

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
    GLuint PixelFormat; // Holds The Results After Searching For A Match
    WNDCLASS wc; // Windows Class Structure
    DWORD dwExStyle; // Window Extended Style
    DWORD dwStyle; // Window Style

    RECT WindowRect; // Grabs Rectangle Upper Left / Lower Right Values
    WindowRect.left=(long)0; // Set Left Value To 0
    WindowRect.right=(long)width; // Set Right Value To Requested Width
    WindowRect.top=(long)0; // Set Top Value To 0
    WindowRect.bottom=(long)height; // Set Bottom Value To Requested Height

    hInstance = GetModuleHandle(NULL); // Grab An Instance For Our Window
    wc.style  = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // Redraw On Move, And Own DC For Window
    wc.lpfnWndProc = (WNDPROC) WndProc; // WndProc Handles Messages
    wc.cbClsExtra = 0; // No Extra Window Data
    wc.cbWndExtra = 0; // No Extra Window Data
    wc.hInstance = hInstance; // Set The Instance
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // Load The Default Icon
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Load The Arrow Pointer
    wc.hbrBackground = NULL; // No Background Required For GL
    wc.lpszMenuName = NULL; // We Don't Want A Menu
    wc.lpszClassName = "OpenGL"; // Set The Class Name

    if (!RegisterClass(&wc)) // Attempt To Register The Window Class
    {
        MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }



    dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE; // Window Extended Style
    dwStyle=WS_OVERLAPPEDWINDOW; // Windows Style


    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle); // Adjust Window To True Requested Size
    if (!(hWnd=CreateWindowEx(  dwExStyle, // Extended Style For The Window
                                "OpenGL", // Class Name
                                title, // Window Title
                                WS_CLIPSIBLINGS | // Required Window Style
                                WS_CLIPCHILDREN | // Required Window Style
                                dwStyle, // Selected Window Style
                                0, 0, // Window Position
                                WindowRect.right-WindowRect.left, // Calculate Adjusted Window Width
                                WindowRect.bottom-WindowRect.top, // Calculate Adjusted Window Height
                                NULL, // No Parent Window
                                NULL, // No Menu
                                hInstance, // Instance
                                NULL))) // Don't Pass Anything To WM_CREATE
    {
        KillGLWindow(); // Reset The Display
        MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    static  PIXELFORMATDESCRIPTOR pfd= // pfd Tells Windows How We Want Things To Be
    {
    sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
    1, // Version Number
    PFD_DRAW_TO_WINDOW | // Format Must Support Window
    PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
    PFD_DOUBLEBUFFER, // Must Support Double Buffering
    PFD_TYPE_RGBA, // Request An RGBA Format
    static_cast<BYTE>(bits), // Select Our Color Depth
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

    if (!(hDC=GetDC(hWnd))) // Did We Get A Device Context?
    {
        KillGLWindow(); // Reset The Display
        MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE; // Return FALSE
    }

    if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd))) // Did Windows Find A Matching Pixel Format?
    {
        KillGLWindow(); // Reset The Display
        MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE; // Return FALSE
    }

    if(!SetPixelFormat(hDC,PixelFormat,&pfd)) // Are We Able To Set The Pixel Format?
    {
        KillGLWindow(); // Reset The Display
        MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE; // Return FALSE
    }

    if (!(hRC=wglCreateContext(hDC))) // Are We Able To Get A Rendering Context?
    {
        KillGLWindow(); // Reset The Display
        MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE; // Return FALSE
    }

    if(!wglMakeCurrent(hDC,hRC)) // Try To Activate The Rendering Context
    {
        KillGLWindow(); // Reset The Display
        MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE; // Return FALSE
    }

    ShowWindow(hWnd,SW_SHOW); // Show The Window
    SetForegroundWindow(hWnd); // Slightly Higher Priority
    SetFocus(hWnd); // Sets Keyboard Focus To The Window
    ReSizeGLScene(width, height);  // Set Up Our Perspective GL Screen

    if (!InitGL()) // Initialize Our Newly Created GL Window
    {
        KillGLWindow(); // Reset The Display
        MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    return TRUE;
}

LRESULT CALLBACK WndProc( HWND hWnd, // Handle For This Window
                          UINT uMsg, // Message For This Window
                          WPARAM wParam, // Additional Message Information
                          LPARAM lParam) // Additional Message Information
{
    switch (uMsg)                               // Check For Windows Messages
    {
        case WM_ACTIVATE:                       // Watch For Window Activate Message
        {
            if (!HIWORD(wParam))                    // Check Minimization State
            {
                active=TRUE;                    // Program Is Active
            }
            else
            {
                active=FALSE;                   // Program Is No Longer Active
            }

            return 0;                       // Return To The Message Loop
        }
        case WM_SYSCOMMAND:                     // Intercept System Commands
        {
            switch (wParam)                     // Check System Calls
            {
                case SC_SCREENSAVE:             // Screensaver Trying To Start?
                case SC_MONITORPOWER:               // Monitor Trying To Enter Powersave?
                    return 0;                   // Prevent From Happening
            }
            break;                          // Exit
        }
        case WM_CLOSE:                          // Did We Receive A Close Message?
        {
            PostQuitMessage(0);                 // Send A Quit Message
            return 0;                       // Jump Back
        }
        case WM_KEYDOWN:                        // Is A Key Being Held Down?
        {
            keys[wParam] = TRUE;                    // If So, Mark It As TRUE
            return 0;                       // Jump Back
        }
        case WM_KEYUP:                          // Has A Key Been Released?
        {
            keys[wParam] = FALSE;                   // If So, Mark It As FALSE
            return 0;                       // Jump Back
        }
        case WM_SIZE:                           // Resize The OpenGL Window
        {
            ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));       // LoWord=Width, HiWord=Height
            return 0;                       // Jump Back
        }
    }
    return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    MSG msg; // Windows Message Structure
    BOOL done=FALSE; // Bool Variable To Exit Loop


    // Create Our OpenGL Window
    if (!CreateGLWindow("QWindow",640,480,16,false))
    {
        return 0; // Quit If Window Was Not Created
    }

    while(!done) // Loop That Runs Until done=TRUE
    {
        if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))           // Is There A Message Waiting?
        {
            if (msg.message==WM_QUIT)// Have We Received A Quit Message?
            {
                done=TRUE; // If So done=TRUE
            }
            else // If Not, Deal With Window Messages
            {
                TranslateMessage(&msg);             // Translate The Message
                DispatchMessage(&msg);              // Dispatch The Message
            }
        }
        else
        {
            if (active) // Program Active?
            {
                if (keys[VK_ESCAPE]) // Was ESC Pressed?
                {
                    done=TRUE; // ESC Signalled A Quit
                }
                else // Not Time To Quit, Update Screen
                {
                    DrawGLScene(); // Draw The Scene
                    SwapBuffers(hDC); // Swap Buffers (Double Buffering)
                }
            }
            if (keys[VK_F1])                    // Is F1 Being Pressed?
            {
                keys[VK_F1]=FALSE; // If So Make Key FALSE
                KillGLWindow(); // Kill Our Current Window
                // Recreate Our OpenGL Window
                if (!CreateGLWindow("QWindow",640,480,16,false))
                {
                    return 0; // Quit If Window Was Not Created
                }
            }
        }
    }
    KillGLWindow(); // Kill The Window
    return (msg.wParam); // Exit The Program
}