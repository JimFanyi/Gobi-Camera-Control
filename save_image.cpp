#include "stdio.h" // C Standard Input/Output library.
#include "XCamera.h" // Xeneth SDK main header.
#include <string>
#include "windows.h"


int save_image(XCHANDLE handle, int imageNumber, const char* dir)
{
    // Variables
    //XCHANDLE handle = 0; // Handle to the camera
    ErrCode errorCode = 0; // Used to store returned errorCodes from the SDK functions.
    word* frameBuffer = 0; // 16-bit buffer to store the capture frame.
    dword frameSize = 0; // The size in bytes of the raw image.

    std::string directory =dir;
    std::string Number = std::to_string(imageNumber);
    std::string append = ".png";
    std::string savename = directory + Number + append;
    const char* savings = savename.c_str();

  
    // When the connection is initialised, ...
    if (XC_IsInitialised(handle))
    {
        // ... start capturing
        //printf("Start capturing.\n");

       if (XC_IsCapturing(handle)) // When the camera is capturing ...
        {

            /*
            *  The calibration pack.
            *  Calibration packs are camera specific.
            *  Copy the calibration pack that comes with the camera onto your system and modify the packname variable accordingly.
            */
            //XC_LoadCalibration(handle, "pack.xca", XLC_StartSoftwareCorrection);

            // Load thermography filter
            //int thermalFilterId = XC_FLT_Queue(handle, "Thermography", 0);

            // Determine native framesize.
            frameSize = XC_GetFrameSize(handle);

            // Initialize the 16-bit buffer.
            frameBuffer = new word[frameSize/2];

            // ... grab a frame from the camera.

       
                if ((errorCode = XC_GetFrame(handle, FT_NATIVE, XGF_Blocking, frameBuffer, frameSize)) != I_OK)
                {
                    printf("Problem while fetching frame, errorCode %lu", errorCode);

                }

                errorCode = XC_SaveData(handle, savings, XSD_Force16);

               
            

        }
    }
    else
    {
        printf("Initialization failed\n");
    }

    // Cleanup.

    // When the camera is still capturing, ...
   /*
     if (XC_IsCapturing(handle))
    {
        // ... stop capturing.
        printf("Stop capturing.\n");
        if ((errorCode = XC_StopCapture(handle)) != I_OK)
        {
            printf("Could not stop capturing, errorCode: %lu\n", errorCode);
        }
    }
   */
  
    /*
    // When the handle to the camera is still initialised ...
    if (XC_IsInitialised(handle))
    {
        printf("Closing connection to camera.\n");
        XC_CloseCamera(handle);
    }

    printf("Clearing buffers.\n");
    */
    if (frameBuffer != 0)
    {
        delete[] frameBuffer;
        frameBuffer = 0;
    }

    return 0;
}
/*
int save_image(XCHANDLE handle, int imageNumber)
{
    // Variables
    ErrCode errorCode = 0; // Used to store returned errorCodes from the SDK functions.

    // Open a connection to the first detected camera by using connection string cam://0

    // When the connection is initialised, ...
    if (XC_IsInitialised(handle))
    {
        std::string directory = "D:\fanyi\Ground-Satellit_Communication\gobicamera_capture\images\gobi_";
        std::string Number = std::to_string(imageNumber);
        std::string savename = directory + Number;
        const char* savings = savename.c_str();
        //const char* settings = "D:\fanyi\Ground-Satellit_Communication\gobicamera_capture\images\gobi_"+std::to_string(imageNumber).c_str();
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
        /*printf("saving image.\n");
        XC_SaveData(handle, savings, XSD_Force16);

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
}*/
