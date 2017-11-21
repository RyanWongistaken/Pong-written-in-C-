/////////////////////////////////////////////////////////////////////////////
///
///		CPong		by: Ryan Wong
///
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CPong.h"
#include <thread>

/** @brief enables multi-threading and creates the canvas with the specified height and width
*
* @parameter the width of the canvas
* @parameter the height of the canvas
* @return 
*/
CPong::CPong(int width, int height)
{
	canvas = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);
	_thread_exit = false;
	
}

/** @brief nothing
*
* @parameter takes no parameters
* @return nothing
*/
CPong::~CPong()
{
	
}

/** @brief calculates paddle and ball positions as well as keeps track of points
*
* @parameter takes no parameters
* @return nothing
*/
void CPong::update()
{

	Arduino.get_data(ANALOG, 1, ADC1);
	Arduino.get_data(ANALOG, 2, ADC2);
	mtx.lock();
	player1_position = (ADC1 * 100) / 720;
	player2_position = (ADC2 * 100) / 720;
	mtx.unlock();

	if (player1_score >= 10 || player2_score >= 10)
	{
		_thread_exit = true;
	}



	if (direction == 0)
	{
		x2 = x1 + 5;
		if (up_or_down == 0)
		{
			y2 = y1 + 5;
		}
		else
		{
			y2 = y1 - 5;
		}


	}
	else
	{
		x2 = x1 - 5;
		if (up_or_down == 0)
		{
			y2 = y1 + 5;
		}
		else
		{
			y2 = y1 - 5;
		}

	}
	//right paddle  hit ditection
	if (x1 >= (canvas.size().width * 0.97) && y1 <= (canvas.size().height * ((player2_position / 100) + 0.06)) && y1 >= (canvas.size().height * ((player2_position / 100) - 0.06)))
	{
		direction = 1;
	}
	//left paddle hit ditection
	else if (x1 <= (canvas.size().width * 0.03) && y1 <= (canvas.size().height * ((player1_position / 100) + 0.06)) && y1 >= (canvas.size().height * ((player1_position / 100) - 0.06)))
	{
		direction = 0;
	}
	//point counting
	if (x1 <= 0)
	{
		player2_score++;
		x2 = 0.3 * canvas.size().width;
		y2 = 0.3 * canvas.size().height;
		direction = 0;
	}
	else if (x1 >= canvas.size().width)
	{
		player1_score++;
		x2 = 0.7 * canvas.size().width;
		y2 = 0.7 * canvas.size().height;
		direction = 1;
	}


	//bounces the ball
	if (y2 >= canvas.size().height)
	{
		up_or_down = 1;
	}
	else if (y2 <= 0)
	{
		up_or_down = 0;
	}
	x1 = x2;
	y1 = y2;


}

/** @brief draws the ball, the paddles, the points and the fps to the screen
*
* @parameter takes no parameters
* @return nothing
*/
void CPong::draw()
{
	using namespace std;

	auto start_time = std::chrono::steady_clock::now();
	auto end_time = start_time + frame_duration(1);

	freq = cv::getTickFrequency(); // Get tick frequency
	start_tic = cv::getTickCount(); // Get number of ticks since event (such as computer on)

	//clear last input
	cv::Point stop = cv::Point(canvas.size().width, canvas.size().height);
	cv::Point start = cv::Point(0, 0);
	cv::rectangle(canvas, start, stop, cv::Scalar(0, 0, 0), CV_FILLED);

	//player 1 position
	cv::Point start1 = cv::Point(canvas.size().width * 0.01, canvas.size().height * ((player1_position / 100) - 0.06));
	cv::Point stop1 = cv::Point(canvas.size().width * 0.03 , canvas.size().height * ((player1_position / 100) + 0.06));
	cv::rectangle(canvas, start1, stop1, cv::Scalar(200, 200, 200), CV_FILLED);

	//player 2 position
	cv::Point start2 = cv::Point(canvas.size().width * 0.99, canvas.size().height * ((player2_position / 100) - 0.06));
 	cv::Point stop2 = cv::Point(canvas.size().width * 0.97, canvas.size().height * ((player2_position / 100) + 0.06));
	cv::rectangle(canvas, start2, stop2, cv::Scalar(200, 200, 200), CV_FILLED);

	//draw the ball
	cv::Point ball = cv::Point(x2,y2);
	cv::circle(canvas, ball, canvas.size().width * 0.01, cv::Scalar(200, 200, 200), CV_FILLED);
	

	
	//fps 
	string fps_string;
	fps_counter = 1 / elapsed_time;
	cv::Point fps_place = cv::Point(canvas.size().width * 0.80, canvas.size().height * 0.9);
	stringstream fps;
	fps << "FPS: " << (1 / elapsed_time);
	
	cv::putText(canvas, fps.str(), fps_place, CV_FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(200, 200, 200));

	//points
	string player1points_str;
	string player2points_str;
	stringstream player2points_intermediate;
	stringstream player1points_intermediate;
	player2points_intermediate << "Points: " << player2_score;
	player1points_intermediate << "Points: " << player1_score;
	cv::Point score1_place = cv::Point(canvas.size().width * 0.09, canvas.size().height * 0.05);
	cv::Point score2_place = cv::Point(canvas.size().width * 0.8, canvas.size().height * 0.05);
	cv::putText(canvas, player1points_intermediate.str(), score1_place, CV_FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(200, 200, 200));
	cv::putText(canvas, player2points_intermediate.str(), score2_place, CV_FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(200, 200, 200));



	
	cv::imshow("Image", canvas);
	cv::waitKey(1);
	
	std::this_thread::sleep_until(end_time);
	elapsed_time = (cv::getTickCount() - start_tic) / freq;

	//cout << "Player 1 Score: " << setw(2) << player1_score << " Player 2 Score : " << setw(2) << player2_score << " fps: " << (1 / elapsed_time) << endl;
}

/** @brief enables threads to acess to acess the method update
*
* @parameter pointer to the class
* @return returns 0
*/
UINT CPong::update_thread(CPong* ptr) 
{
	while (ptr->_thread_exit == false) 
	{
		ptr->update(); 
	}
	return 0;
}

/** @brief enables threads to acess to acess the method update
*
* @parameter pointer to the class
* @return nothing
*/
UINT CPong::draw_thread(CPong* ptr)
{
	while (ptr->_thread_exit == false)
	{

		ptr->draw();
	
	}
	return 0;
}

/** @brief
*
* @parameter takes no parameters
* @return nothing
*/
void CPong::start_thread()
{
		std::thread t1(&CPong::update_thread, this);
		std::thread t2(&CPong::draw_thread, this);
		t1.detach();
		t2.detach();
}


/** @brief Multi-threads the two methods update and draw
*
* @parameter takes no parameters
* @return nothing
*/
void CPong::run()
{
	using namespace std;
	CPong::start_thread();
	while (player1_score < 10 && player2_score < 10);
	{
		std::cout << "gg ez" << endl;
	}
	_thread_exit = true;
	cv::waitKey(100);

}