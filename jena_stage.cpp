#pragma once
#include <cstdlib>
#include <iostream>
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

class Stage{
public:
  Stage(double step, double center, double start_point, serial::Serial *my_serial);
  void piezo_initializer();
  void move_onestep();
  void exit();
private:
  serial::Serial* my_serial;
  double split(string s);
  string exec_command(string command);
  double get_position();
  void set_position(double position);
  double step_length;
  double trace_center;
  double start_point;

};

serial::Serial my_serial("/dev/ttyUSB0", 9600, serial::Timeout::simpleTimeout(1000));

Stage::Stage(double step, double center, double start_point, serial::Serial *my_serial):step_length (step), trace_center(center), start_point(start_point), my_serial(my_serial){}

double Stage::split(string s){
    size_t pos1 = s.find(",");
    size_t pos2 = s.find("\n");
    return stod(s.substr(pos1+1,pos2-pos1-1));
}

string Stage::exec_command(string command){
    my_serial->write(command);
    return my_serial->read(100);
}

void Stage::piezo_initializer(){
    exec_command("i1\r");
    return;
}

double Stage::get_position(){
    string s = exec_command("rd\r");
    return split(s);
}

void Stage::set_position(double position){
    exec_command("wr,"+to_string(position)+"\r");
    return;
}

void Stage::move_onestep(){
    set_position(get_position() + step_length);
    return;
}

void Stage::exit(){
    exec_command("i0\r");
    return;
}

extern Stage stage;