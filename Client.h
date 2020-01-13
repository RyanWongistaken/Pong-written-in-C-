/**
*
* @brief Made to demonstrate client/Server code by Craig Hennessey 
*
* @author Ryan Wong
*
*/
#pragma once

#include <string>

#include "opencv.hpp"

#include "Winsock2.h"

#define BUFF_SIZE 65535

class Client 
{
private:
  WSADATA _wsdat; 

	int _port;
  std::string _addr;
  SOCKET _socket;

  char rxbuff[BUFF_SIZE];
public:
  Client(int port, std::string addr);
	~Client();

  void tx_str(std::string str);
  bool rx_str(std::string &str);
  bool rx_im(cv::Mat &im);
};


