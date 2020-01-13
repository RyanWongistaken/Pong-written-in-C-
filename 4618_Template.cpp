/**
*
* @brief Using OpenCV template by Craig to make a game of Pong using C++
*
* @author Ryan Wong
*
*/

#include "stdafx.h"
#include <string>
#include <iostream>
#include <thread>
#include "CPong.h"
#include "Serial.h" // Must include Windows.h after Winsock2.h, so Serial must include after Client/Server

// OpenCV Include
#include "opencv.hpp"

// OpenCV Library
#pragma comment(lib,".\\opencv\\lib\\opencv_world310d.lib")

int main(int argc, char* argv[])
{
	CPong game(900, 750); // Create Game Object
	game.run(); // Run Game
}
