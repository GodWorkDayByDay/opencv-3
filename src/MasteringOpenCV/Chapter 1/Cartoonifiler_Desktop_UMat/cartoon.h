#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>

#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

void cartoonifyImage(UMat srcColor, UMat dst, bool sketmode, bool alienmode, bool evilmode, int debugType);
void drawFaceStickFigure(UMat dst);

void changeFacialskincolor(UMat smallImageBGR, Mat bigEdges, int debugType);

void removePepperNoise(Mat& mask);