/*
* AppInitialize函数，返回整型，不需要传递参数
* 完成图片存储路径设置、拍摄间隔设置等工作
*/

#include "iostream" // C Standard Input/Output library.
#include "XCamera.h" // Xeneth SDK main header.
#include <windows.h>
#include "myfunctions.h"
#include "variables.h"

int ImageCapture(){
    const char* dir = choose_dir();
    //functions
    // Open a connection to the first detected camera by using connection string cam://0
    // Note the fg parameter that is passed in the query part of the connection string.
    printf("请输入间隔时间： ");
    scanf_s("%d", &a);

    printf("/-----------------------打开相机连接-----------------------/\n");
    printf("Opening connection to cam://0\n");
    //printf("Opening connection to cam://0, using the first detected CameraLink transport CL0.\n\n");
    printf("/---------------------------------------------------------/\n");

    handle = XC_OpenCamera("cam://0");
    load_settings(handle);
    printf("/-----------------------相机设置检查-----------------------/\n");
    disp_settings(handle);
    printf("Output dirctory: %s\n", dir);
    printf("/----------------------------------------------------------/\n");

    printf("Press any key to start capture\n");
    system("pause");
    
    return 0;
}