// sample2_hellow_world.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <Windows.h>

int main(int agrc, char* args[])
{
	cv::Mat img(480, 640, CV_8U);
	cv::putText(
		img,
		"Hellow world!",
		cv::Point(200,400),
		cv::FONT_HERSHEY_SIMPLEX|cv::FONT_ITALIC,
		1.0,
		cv::Scalar(0,0,0)
	);

	const char* window_name = "My Window";
	cv::imshow(window_name, img);
	cv::waitKey();

    return 0;
}

