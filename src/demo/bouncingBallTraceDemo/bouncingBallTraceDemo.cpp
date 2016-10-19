// bouncingBallTraceDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <vector>

void searchBall(cv::Mat& thresholdImage, cv::Mat& camera)
{
	cv::Mat temp;
	thresholdImage.copyTo(temp);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(temp, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	if (contours.empty()) return;

	std::vector<std::vector<cv::Point>> largestContourVec;
	largestContourVec.push_back(contours[contours.size() - 1]);

	cv::Rect boundRect = cv::boundingRect(largestContourVec[0]);

	int x = boundRect.x + boundRect.width / 2;
	int y = boundRect.y + boundRect.height/ 2;

	cv::circle(camera, cv::Point(x, y), 20, cv::Scalar(0, 255, 0), 2);
}

int main()
{
	cv::VideoCapture cap;
	cv::Mat frame1, frame2;
	cv::Mat gray1, gray2, diffgray, thresgray;

	while (true) {
		cap.open("../../../images/768x576.avi");
		if (!cap.isOpened()) return -1;
		while (
			cap.get(cv::CAP_PROP_POS_FRAMES) < cap.get(cv::CAP_PROP_FRAME_COUNT) - 1
			) {
			cap >> frame1;
			cv::cvtColor(frame1, gray1, cv::COLOR_BGR2GRAY);

			cap >> frame2;
			cv::cvtColor(frame2, gray2, cv::COLOR_BGR2GRAY);

			cv::absdiff(gray1, gray2, diffgray);
			cv::threshold(diffgray, thresgray, 20, 255, cv::THRESH_BINARY);

			searchBall(thresgray, frame1);
			cv::imshow("final image", thresgray);
			cv::imshow("org image", frame1);

			auto key = cv::waitKey(10);
			if (VK_ESCAPE == key) return 0;
		}
	}
    return 0;
}

