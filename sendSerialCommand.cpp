#define DATA_LENGTH 255

#include "SerialPort.hpp"
#include "SerialPort.cpp"
#include <iostream>

const char *portName = "\\\\.\\COM20";

//Declare a global object
SerialPort *arduino;

//Here '\n' is a delimiter 
const char *sendString = "Hello World\n"; 

void sendSerialCommand()
{
  arduino = new  SerialPort(portName);
  if (arduino->isConnected()){
    bool hasWritten = arduino->writeSerialPort(sendString, DATA_LENGTH);
    if (hasWritten) std::cout << "Data Written Successfully" << std::endl;
    else std::cerr << "Data was not written" << std::endl;
  }
}