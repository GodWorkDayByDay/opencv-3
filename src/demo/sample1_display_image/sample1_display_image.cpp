// sample1_display_image.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, const char* argv[])
{
	if (2 != argc) {
		std::cout << "usage:DisplayImage.out<Image_Path>\n";
		return -1;
	}

	cv::Mat image;
	image = cv::imread(argv[1], cv::IMREAD_COLOR);
	if (!image.data) {
		std::cout << "No image data\n";
		return -1;
	}

	const char* window_name = "Display Image";
	cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
	cv::imshow(window_name, image);

	cv::waitKey(0);

    return 0;
}

