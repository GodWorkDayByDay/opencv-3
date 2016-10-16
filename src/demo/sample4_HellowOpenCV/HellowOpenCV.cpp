// HellowOpenCV.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <Windows.h>

int main()
{
	cv::VideoCapture cap;
	cap.open(0);
	if (!cap.isOpened()) {
		std::cout << "can not open camera!!!" << std::endl;
		return -1;
	}

	cv::Mat kern = (
		cv::Mat_<char>(3, 3) 
		<< 
		0, -1, 0,
		-1, 0, 1,
		0, 1, 0
		);

	while (1) {
		cv::Mat frame;
		cap >> frame;
		if (frame.empty()) {
			std::cout << "image not found.\n";
			return -1;
		}

		cv::Mat con_frame;
		cv::filter2D(frame, con_frame, frame.depth(), kern);

		cv::imshow("test", frame);
		cv::imshow("test con_frame", con_frame);
		auto key = cv::waitKey(10);
		if (VK_ESCAPE == key) break;
	}

    return 0;
}

