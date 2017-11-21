///////////////////////////////////////////////////////////
//
//		CControl methodes by Ryan Wong
//
////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CControl.h"
#include <sstream>


/** @brief retrieves data from the arduino based on the input parameters
*
* @parameter the type is either (digital, analog or servo)
* @parameter which channel to be read from
* @parameter the value read is passed by reffrence
*
* @return returns true if the data could be aquired
*/
bool CControl::get_data(int type, int channel, int &value)
{
	using namespace std;
	arduino.flush();
	string command;
	string escrow;
	int timer = 0;
	int valuefromadc = 0;
	command = "G " + to_string(type) + " " + to_string(channel) + " \n";
	string rx_str;
	

	// temporary storage
	char buff[2];
	
	rx_str = "";
	arduino.write(command.c_str(), command.length());
	float start_time = GetTickCount();

	buff[0] = 0;
	// If 1 byte was read then print to screen, timeout after 1 second
	while (buff[0] != '\n' && GetTickCount() - start_time < 1000)
	{
		float start_time2 = GetTickCount();
		if (arduino.read(buff, 1) > 0)
		{
			rx_str = rx_str + buff[0];
		}
		timer = GetTickCount() - start_time2;
	}

	escrow = rx_str;
	escrow.erase(0, 6);
	stringstream intermediate(escrow);
	intermediate >> valuefromadc;
		
	value = valuefromadc;

	
	if (timer <= 1000)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/** @brief sends data to the aurdino
*
* @parameter the type is either (digital or servo)
* @parameter which channel to be set
* @parameter the value to be written
*
* @return returns true if the data could be aquired
*/
bool CControl::set_data(int type, int channel, int value)
{
	using namespace std;
	arduino.flush();
	string command;
	command = "S " + to_string(type) + " " + to_string(channel) + " " + to_string(value) + " \n";
	arduino.write(command.c_str(), command.length());
	return true;
}

/** @brief enables the right port
*
* @parameter dosnt take a parameter
* @return returns nothing
*/
CControl::CControl()
{
	// Comport class
	arduino.open("COM3");
}

CControl::~CControl()
{

}