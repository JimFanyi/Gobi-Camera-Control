

#include <windows.h>
#include <Commdlg.h>
#include <stdio.h>

OPENFILENAME ofn;
char szFile[300];

const char* choose_dir()
{
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = (LPWSTR)szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetOpenFileName(&ofn))
    {
        wprintf(L"%s\n", ofn.lpstrFile);
    }
    else
    {
        printf("user cancelled\n");
    }
    return (const char*) ofn.lpstrFilter;
}