#include "iostream" // C Standard Input/Output library.
#include "XCamera.h" // Xeneth SDK main header.
#include <windows.h>
#include "myfunctions.h"
#include "variables.h"

int FrameCapture(const char* dir, XCHANDLE handle) {
    ErrCode errorCode = 0; // Used to store returned errorCodes from the SDK functions.
    int imageNumber = 1;
    int ActimageNumber = 1;
    int time_cnt = 0;
    double temp = 0;
    int timeSep = 0;
    int i = 0;
    while (XC_IsInitialised(handle) && XC_StartCapture(handle) == I_OK)
    {
        if (time_cnt >= 100)
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


        for (i = 0; i < timeSep; i++)
        {
            if (i < (timeSep - 1))
            {
                printf("Sleeping...remain %d seconds\n", (timeSep - i));
                Sleep(1000);
            }
            else if (i == (timeSep - 1))
            {
                printf("Sleeping...remain %d seconds\n\n", (timeSep - i));
                Sleep(1000);
            }

        }

    }

    return 0;
}