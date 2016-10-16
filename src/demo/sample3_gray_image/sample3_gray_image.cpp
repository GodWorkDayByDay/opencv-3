// sample3_gray_image.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, const char* argv[])
{
	if (3 != argc) {
		std::cout << "No image data\n";
		return -1;
	}

	cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR);
	cv::Mat gray_image;
	cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

	cv::imwrite(argv[2], gray_image);

	cv::namedWindow(argv[1], cv::WINDOW_AUTOSIZE);
	cv::namedWindow(argv[2], cv::WINDOW_AUTOSIZE);

	cv::imshow(argv[1], image);
	cv::imshow(argv[2], gray_image);
	cv::waitKey();

    return 0;
}

