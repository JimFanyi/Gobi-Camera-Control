
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
    int a = 0;
    int i = 0;
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



  //properties();




    while (XC_IsInitialised(handle) && XC_StartCapture(handle) == I_OK)
    {
        if (time_cnt >= 100 )
        {
            if ((errorCode = XC_StartCapture(handle)) != I_OK)
            {
                printf("Could not start capturing, errorCode: %lu\n", errorCode);
            }
            printf("/-----------------------相机设置检查-----------------------/\n");
            disp_settings(handle);
        
            printf("/----------------------------------------------------------/\n");
            time_cnt = 1;
        }



        
        temp = temp_read(handle);
        printf("相机温度为%f℃\n", temp);
        
        printf("保存第%d张图片\n", ActimageNumber);
        save_image(handle, imageNumber, dir);
        printf("存储完成\n\n");
        imageNumber++;
        ActimageNumber++;
        time_cnt++;
       

        for (i = 0; i < a; i++)
        {
            if (i < (a - 1))
            {
                printf("Sleeping...remain %d seconds\n", (a - i));
                Sleep(1000);
          }
            else if (i == (a - 1))
            {
                printf("Sleeping...remain %d seconds\n\n", (a - i));
                Sleep(1000);
            }
           
        }
            
    }
    return 0;
}