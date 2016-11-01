// Cartoonifiler_Desktop_UMat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2\opencv.hpp>
#include <Windows.h>
#include "cartoon.h"

const int DESIRED_CAMERA_WIDTH = 640;
const int DESIRED_CAMERA_HEIGHT = 480;
const char* windowName = "Cartoonifier";

bool m_sketchMode = false;
bool m_alienMode = false;
bool m_eviMode = false;
bool m_debugMode = false;

const int NUM_STICK_FIGURE_ITERATIONS = 40;

using namespace cv;
using namespace std;

int m_stickFigureIterations = 0;

void initWebcam(VideoCapture &videoCapture, int cameraNumber) {
	try {
		videoCapture.open(cameraNumber);
	}
	catch (cv::Exception& e) {
	
	}

	if (!videoCapture.isOpened()) {
		cerr << "ERROR: could not access the camera!" << endl;
		exit(1);
	}
	cout << "Loaded camera " << cameraNumber << "." << endl;
}

void onKeypress(char key)
{
	switch (key)
	{
	case 's':
		m_sketchMode = !m_sketchMode;
		cout << "Sketch / Paint mode : " << m_sketchMode << endl;
		break;

	case 'a':
		m_alienMode = !m_alienMode;
		cout << "Alien / Humman mode: " << m_alienMode << endl;
		if (m_alienMode) {
			m_stickFigureIterations = NUM_STICK_FIGURE_ITERATIONS;
		}
		break;

	case 'e':
		m_eviMode = !m_eviMode;
		cout << "Evil / Good mode: " << m_eviMode << endl;
		break;

	case 'd':
		m_debugMode = !m_debugMode;
		cout << "Debug mode : " << m_debugMode << endl;
		break;
	}
}

int main(int argc, char *argv[])
{
	cout << "Cartoonifier, by Shervin Emami (www.shervinemami.info), June 2012." << endl;
	cout << "Converts real-life images to cartoon-like images." << endl;
	cout << "Compiled with OpenCV version " << CV_VERSION << endl;
	cout << endl;

	cout <<
		R"[delimiter](Keyboard commands (press in the GUI window):
	    Esc:  Quit the program.
	    s:    change Sketch / Paint mode.
	    a:    change Alien / Human mode.
	    e:    change Evil / Good character mode.
	    d:    change debug mode.)[delimiter]";

	int cameraNumber = 0;

	if (argc > 1) {
		cameraNumber = atoi(argv[1]);
	}

	VideoCapture camera;
	initWebcam(camera, cameraNumber);

	camera.set(CAP_PROP_FRAME_WIDTH, DESIRED_CAMERA_WIDTH);
	camera.set(CAP_PROP_FRAME_HEIGHT, DESIRED_CAMERA_HEIGHT);

	namedWindow(windowName);

	UMat cameraFrame;
	UMat displayFrame;

	int debugType = 0;
	if (m_debugMode) debugType = 1;

	while (true) {
		camera >> cameraFrame;
		if (cameraFrame.empty()) {
			cerr << "ERROR: Couldn't grab the next camera frame." << endl;
			exit(1);
		}

		if (displayFrame.empty()) {
			displayFrame = UMat(cameraFrame.size(), CV_8UC3);
			// unsigned 8bit 3 channels = 8UC x 3 = (r,g,b) rgb coolor model (중용해지고있음.)
		}

		cartoonifyImage(cameraFrame, displayFrame, m_sketchMode, m_alienMode, m_eviMode, m_debugMode);

		if (m_stickFigureIterations > 0) {
			drawFaceStickFigure(displayFrame);
			m_stickFigureIterations--;
		}
		imshow(windowName, displayFrame);

		char keypress = waitKey(20);
		if (keypress == VK_ESCAPE) {
			break;
		}
		if (keypress <= 0) continue;

		onKeypress(keypress);
	}

    return EXIT_SUCCESS;
}

