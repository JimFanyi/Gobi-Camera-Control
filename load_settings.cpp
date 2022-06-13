#include "stdio.h" // C Standard Input/Output library.
#include "XCamera.h" // Xeneth SDK main header.

int load_settings(XCHANDLE handle)
{
    // Variables
    ErrCode errorCode = 0; // Used to store returned errorCodes from the SDK functions.

    // Open a connection to the first detected camera by using connection string cam://0

    // When the connection is initialised, ...
    if (XC_IsInitialised(handle))
    {   
        const char* settings = "D:\\fanyi\\Ground-Satellit_Communication\\gobicamera_capture\\camerasettings.xcf";
       // sprintf(settings, "D:\fanyi\Ground-Satellit_Communication\gobicamera_capture\images\gobi");
        long oldint = 0;
        long newint = 32;
/*
   // Save settings
        printf("Saving settings.\n");
        XC_SaveSettings(handle, settings);
*/
     

        // Change a property
        //errorCode = XC_GetPropertyValueL(handle, "IntegrationTime", &oldint);
        //errorCode = XC_SetPropertyValueL(handle, "IntegrationTime", newint, "");
        //printf("- Saved IntegrationTime value: %ld\n- New IntegrationTime value: %ld\n", oldint, newint);

        // Load settings.
        printf("Load settings.\n");
        XC_LoadSettings(handle, settings);

        // Verify new value..
        oldint = 0;
        errorCode = XC_GetPropertyValueL(handle, "IntegrationTime", &oldint);
        printf("- Loaded IntegrationTime value: %ld\n", oldint);
    }
    else
    {
        printf("Initialization failed\n");
    }

    // Cleanup.

    // When the handle to the camera is still initialised ...


    return 0;
}
