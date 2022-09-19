
#include "iostream" // C Standard Input/Output library.
#include "XCamera.h" // Xeneth SDK main header.
#include <windows.h>
#include "myfunctions.h"



int main()
{
    // Variables
    XCHANDLE handle = 0; // Handle to the camera
    ErrCode errorCode = 0; // Used to store returned errorCodes from the SDK functions.
    int imageNumber = 1;
    int ActimageNumber = 1;
    int time_cnt = 0;
    double temp = 0;
    int timeSep = 0;
    int i = 0;
    const char* dir;

    printf("请输入间隔时间： ");
    scanf_s("%d", &timeSep);
    dir = choose_dir();
    printf("Output dirctory: %s\n", dir);
   
    handle = CameraInitialize();



  //properties();


    FrameCapture(dir,handle);

    return 0;
}