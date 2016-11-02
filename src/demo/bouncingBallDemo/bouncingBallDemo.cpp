// bouncingBallDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <opencv2/opencv.hpp>

int main()
{
	cv::VideoCapture cap;
	cv::Mat frame1, gray1;
	cv::Mat frame2, gray2;
	cv::Mat diffgray, thresgray;

	while (true) {
		cap.open("768x576.avi");
		if (!cap.isOpened()) return -1;
		while (
			cap.get(cv::CAP_PROP_POS_FRAMES) < cap.get(cv::CAP_PROP_FRAME_COUNT)-1){
			// first image
			cap >> frame1;
			cv::cvtColor(frame1, gray1, cv::COLOR_BGR2GRAY);

			// second image
			cap >> frame2;
			cv::cvtColor(frame2, gray2, cv::COLOR_BGR2GRAY);

			cv::absdiff(gray1, gray2, diffgray);
			cv::threshold(diffgray, thresgray, 20, 255, cv::THRESH_BINARY);
			cv::imshow("final image", thresgray);
			cv::imshow("org image", frame1);

			auto key = cv::waitKey(10);
			if (VK_ESCAPE == key) return 0;
		}
		cap.release();
	}

    return 0;
}

