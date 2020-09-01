#include <cstdlib>
#include <iostream>
#include <string>
#include <windows.h>
#include <PI_GCS2_DLL_PF.h>

using namespace std;

int test(){
    //const char* model = "E-709";
    const char* model = "119064403";
    char* szbuffer;
    int bsize=1;
    int baudrate = 115200;
    HMODULE hPI_Dll = LoadLibrary("PI_GCS2_DLL.dll");
    PFPI_ConnectUSB PI_ConnectUSB;
    PI_ConnectUSB(model);
    return 0;
}