// TempoDetection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "MotionTrack.h"


using namespace cv;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	string imageName("nyc_test.jpg");
	Mat image;
	image = imread(imageName.c_str(), IMREAD_COLOR); // Read the file

	if (!image.data) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	resize(image, image, Size(image.cols /4, image.rows /4));
	namedWindow("Display window", CV_WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display window", image); // Show our image inside it.

	waitKey(0); // Wait for a keystroke in the window
	return 0;
}

