#include "iostream" // C Standard Input/Output library.
#include "XCamera.h" // Xeneth SDK main header.
#include <windows.h>
#include "myfunctions.h"

#include "variables.h"

XCHANDLE CameraInitialize() {



    XCHANDLE handle_inFunction = 0; // Handle to the camera
    ErrCode errorCode = 0; // Used to store returned errorCodes from the SDK functions.
    int imageNumber = 1;
    int ActimageNumber = 1;
    int time_cnt = 0;

    //functions
    // Open a connection to the first detected camera by using connection string cam://0
    // Note the fg parameter that is passed in the query part of the connection string.

    printf("/-----------------------打开相机连接-----------------------/\n");
    printf("Opening connection to cam://0\n");
    //printf("Opening connection to cam://0, using the first detected CameraLink transport CL0.\n\n");
    printf("/---------------------------------------------------------/\n");

    handle_inFunction = XC_OpenCamera("cam://0");
    load_settings(handle_inFunction);
    printf("/-----------------------相机设置检查-----------------------/\n");
    disp_settings(handle_inFunction);
    printf("/----------------------------------------------------------/\n");

    printf("Press any key to start capture\n");
    system("pause");

    return handle_inFunction;
}