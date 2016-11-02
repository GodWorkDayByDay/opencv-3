// sample5_addWeighted.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <string>
#include <chrono>
#include <iomanip>

#if defined(_MSC_VER) || defined(WIN32)  || defined(_WIN32) || defined(__WIN32__) \
    || defined(WIN64)    || defined(_WIN64) || defined(__WIN64__) 

#include <windows.h>
bool _qpcInited = false;
double PCFreq = 0.0;
__int64 CounterStart = 0;
void InitCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
	{
		std::cout << "QueryPerformanceFrequency failed!\n";
	}
	PCFreq = double(li.QuadPart) / 1000.0f;
	_qpcInited = true;
}
double CLOCK()
{
	if (!_qpcInited) InitCounter();
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart) / PCFreq;
}

#endif

#if defined(unix)        || defined(__unix)      || defined(__unix__) \
    || defined(linux)       || defined(__linux)     || defined(__linux__) \
    || defined(sun)         || defined(__sun) \
    || defined(BSD)         || defined(__OpenBSD__) || defined(__NetBSD__) \
    || defined(__FreeBSD__) || defined __DragonFly__ \
    || defined(sgi)         || defined(__sgi) \
    || defined(__MACOSX__)  || defined(__APPLE__) \
    || defined(__CYGWIN__) 
double CLOCK()
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return (t.tv_sec * 1000) + (t.tv_nsec*1e-6);
}
#endif

double _avgdur = 0;
double _fpsstart = 0;
double _avgfps = 0;
double _fps1sec = 0;

double avgdur(double newdur)
{
	_avgdur = 0.98*_avgdur + 0.02*newdur;
	return _avgdur;
}

double avgfps()
{
	if (CLOCK() - _fpsstart>1000)
	{
		_fpsstart = CLOCK();
		_avgfps = 0.7*_avgfps + 0.3*_fps1sec;
		_fps1sec = 0;
	}
	_fps1sec++;
	return _avgfps;
}


int main()
{
	cv::VideoCapture cap;
	cap.open(0);
	if (!cap.isOpened()) {
		std::cout << "can not open camera.\n";
		return -1;
	}
	//cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);

	cv::Mat logo = cv::imread("..\\..\\..\\images\\logo2.png", cv::IMREAD_COLOR);

	while (true) {
		double dStart = CLOCK();
		cv::Mat frame;
		cap >> frame;
		if (frame.empty()) {
			std::cout << "no image\n";
			return -1;
		}
		double dur = CLOCK() - dStart;
		
		// display logo
		cv::Mat roi = frame(cv::Rect(0, 0, logo.size().width, logo.size().height));
		cv::addWeighted(roi, 0.3, logo, 0.7, 0.0, roi);
		
		// display fps
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << "avg time per frame : " << avgdur(dur) << " ms. fps : " << avgfps();
		cv::putText( frame, ss.str(), cv::Point(280, 20), cv::FONT_HERSHEY_SIMPLEX | cv::FONT_ITALIC, 0.5, cv::Scalar(55,55,255));
		//cv::namedWindow("view", CV_WINDOW_AUTOSIZE);
		cv::imshow("view", frame);
		auto key = cv::waitKey(10);
		if (VK_ESCAPE == key) break;
	}

    return 0;
}

