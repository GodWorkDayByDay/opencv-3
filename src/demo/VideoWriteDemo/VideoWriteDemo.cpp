// VideoWriteDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <opencv2/opencv.hpp>


int main()
{
	cv::VideoCapture cap{0};
	if (!cap.isOpened()) return -1;

	std::string filename = "test.avi";
	//https://www.fourcc.org/
	int fourcc = CV_FOURCC('D', 'I', 'V', '3');
	cv::Size frameSize(
		cap.get(cv::CAP_PROP_FRAME_WIDTH),
		cap.get(cv::CAP_PROP_FRAME_HEIGHT));
	double fps = 20;
	cv::VideoWriter writer{ filename, fourcc, fps, frameSize };
	if (!writer.isOpened()) return -1;

	std::string windowName = "webcam Feed";
	cv::Mat frame;
	while (true) {
		cap >> frame;
		if (frame.empty()) break;
		writer.write(frame);
		cv::imshow(windowName, frame);
	
		auto key = cv::waitKey(5);
		if (VK_ESCAPE == key) break;
	}

    return 0;
}

