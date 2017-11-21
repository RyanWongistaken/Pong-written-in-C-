

#pragma once
#include "Serial.h"
#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include "CBase4618.h"
#include <fstream>
#include <thread>
#include <mutex>
#include <cmath>
// OpenCV Include
#include "opencv.hpp"

// OpenCV Library
#pragma comment(lib,".\\opencv\\lib\\opencv_world310d.lib")

// Specify the desired frame rate (30 FPS)
typedef std::chrono::duration<int, std::ratio<1, 30>> frame_duration;

class CPong : public CBase4618 ///< This class contains all methods and varriables relating to making the pong game work
{
private:
	std::mutex mtx;

	int ADC1 = 0; ///stores the value retrieved from the adc channel 1
	int ADC2 = 0; ///stores the value retrieved from the adc channel 1
	float player1_position; /// contains the position of the paddle for player 1
	float player2_position; /// contains the position of the paddle for player 2
	int player1_score = 0; /// contains the number of points player 1 has 
	int player2_score = 0; /// contains the number of points player 2 has 
	int fps_counter = 0;
	
	//ball location
	int x1 = 250;///starting ball location (x)
	int y1 = 250;///starting ball location (y)
	int x2 = 0;///previous ball location (x)
	int y2 = 0;///previous ball location (y)
	int direction = 0; // 0 is right  :: 1 is left
	int up_or_down = 0;// 0 is up :: 1 is down
	double start_tic, freq, elapsed_time;

	//multi threading
	bool _thread_exit;
	static UINT update_thread(CPong* ptr);
	static UINT draw_thread(CPong* ptr);

public:
	CPong(int width, int height);
	~CPong();
	void update();
	void draw();
	void start_thread();
	void run();


};