#pragma once
double temp_read(XCHANDLE handle);
const char* choose_dir();
int properties();
int load_settings(XCHANDLE handle);
int save_image(XCHANDLE handle, int imageNumber, const char* dir);
int disp_settings(XCHANDLE handle);
int CameraInitialize();
int FrameCapture(const char* dir, XCHANDLE handle);