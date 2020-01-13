/**
*
* @brief Contains the serial object and two pulic methods
*
*contains the serial port object Serial and two public methods which implement the serial
*communication protocol outlined in the course lecture notes
*
* @author Ryan Wong
*
*/
#pragma once	
#include "Serial.h"
#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <fstream>

typedef enum {DIGITAL, ANALOG, SERVO} type;//DIGITAL = 0 ANALOG = 1 SERVO = 2

class CControl /// This class controls the communication between the ardunio adc and the PC
{
private:
	Serial arduino; ///generates the object for serial communications
public:
	CControl();
	~CControl();
	bool get_data(int type, int channel, int &value);
	bool set_data(int type, int channel, int value);
};

