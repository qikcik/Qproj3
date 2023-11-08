#include "gui/nativeController.hpp"
#include "gui/nativeWindow.hpp"
#include "gui/tickable.hpp"

BEGIN_GEN_QOBJ_STATIC_DEF(NativeController,Shred)
selfPtr->fields = std::move(DynamicArray<OwnerPtr<Field>>
{

});
END_GEN_QOBJ_STATIC_DEF()

void NativeController::loop(HINSTANCE in_hInstance, int nCmdShow)
{

    for(auto& winIt : getChildsOfClass_singleDeep<NativeWindow>())
        winIt->initializeNative(in_hInstance,nCmdShow);


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
            for( auto& winIt : getChildsWithNativeInterface_singleDeep<Tickable>())
            {
                winIt.second->handleTick(1 / 60.f); // TODO change to real delta
            }
        }
    }
}
