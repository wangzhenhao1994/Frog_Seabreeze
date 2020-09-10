/***
 * This example expects the serial port has a loopback on it.
 *
 * Alternatively, you could use an Arduino:
 *
 * <pre>
 *  void setup() {
 *    Serial.begin(<insert your baudrate here>);
 *  }
 *
 *  void loop() {
 *    if (Serial.available()) {
 *      Serial.write(Serial.read());
 *    }
 *  }
 * </pre>
 */

#include <string>
#include <iostream>
#include <cstdio>

// OS Specific sleep
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "serial/serial.h"

using std::string;
using std::exception;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
 
void my_sleep(unsigned long milliseconds) {
#ifdef _WIN32
      Sleep(milliseconds); // 100 ms
#else
      usleep(milliseconds*1000); // 100 ms
#endif
}

double split(string s){
    size_t pos1 = s.find(",");
    size_t pos2 = s.find("\n");
    return stod(s.substr(pos1+1,pos2-pos1-1));
}

string exec_command(string command, serial::Serial *my_serial){
    my_serial->write(command);
    return my_serial->read(100);
}

void piezo_initializer(){
    
}

double get_position(serial::Serial *my_serial){
    string s = exec_command("rd\r", my_serial);
    return split(s);
}

void set_position(double position, serial::Serial *my_serial){
    exec_command("wr,"+to_string(position), my_serial);
    return;
}

void move_onestep(double stepsize, serial::Serial *my_serial){
    set_position(get_position(my_serial) + stepsize, my_serial);
    return;
}

int test()
{
    serial::Serial my_serial("/dev/ttyUSB0", 9600, serial::Timeout::simpleTimeout(1000));

    if(my_serial.isOpen())
        cout<<"Successfully initialize the stage!!!"<<endl;
    else
        cout<<"Fail to initialize the stage!!!"<<endl;
    double position = 20.00;
    double step = 1.00;
    move_onestep(step, &my_serial);

    return 0;
}
