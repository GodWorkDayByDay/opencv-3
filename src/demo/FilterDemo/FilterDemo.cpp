// FilterDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <opencv2/opencv.hpp>

int main()
{
	cv::VideoCapture cap;
	cap.open(0);
	if (!cap.isOpened()) return -1;

	cv::Mat frame;
	cv::Mat filter1, filter2, filter3;
	constexpr int KERNEL_LENGTH = 13;
	while (true) {
		cap >> frame;
		if (frame.empty()) return -1;
		cv::imshow("org", frame);
	}

    return 0;
}

