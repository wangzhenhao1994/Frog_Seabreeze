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

    PFPI_ConnectRS232 PI_ConnectRS232;
    f=gSystem->DynFindSymbol("PI_GCS2_DLL.dll", "PI_ConnectRS232");
    PI_ConnectRS232=(PFPI_ConnectRS232) f;
    PI_ConnectRS232(1,baudrate)
    return 0;
}