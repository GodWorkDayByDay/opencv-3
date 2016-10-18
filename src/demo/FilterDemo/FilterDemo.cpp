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
		for (int i = 11; i < KERNEL_LENGTH; i = i + 2) {
			cv::GaussianBlur(frame, filter1, cv::Size(i, i), 0);
		}
		for (int i = 11; i < KERNEL_LENGTH; i = i + 2) {
			cv::medianBlur(frame, filter2, i);
		}
		for (int i = 11; i < KERNEL_LENGTH; i = i + 2) {
			cv::bilateralFilter(frame, filter3, i, i * 2, i / 2);
		}

		cv::imshow("GaussianBlur", filter1);
		cv::imshow("medianBlur", filter2);
		cv::imshow("bilateralFilter", filter3);

		auto key = cv::waitKey(5);
		if (VK_ESCAPE == key) break;
	}

    return 0;
}

