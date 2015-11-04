#include "stdafx.h"
#include "gurneet.h"

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";


/**
* @function CannyThreshold
* @brief Trackbar callback - Canny thresholds input with a ratio 1:3
*/
void CannyThreshold(int, void*)
{
	/// Reduce noise with a kernel 3x3
	blur(src_gray, detected_edges, Size(3, 3));

	/// Canny detector
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);

	/// Using Canny's output as a mask, we display our result
	dst = Scalar::all(0);

	src.copyTo(dst, detected_edges);

	imshow(window_name, dst);

}


/** @function main */
int filter_image()
{
	/// Load an image
	src = imread("test_vid/Capture.png");

	if (!src.data)
	{
		return -1;
	}

	///// Create a matrix of the same type and size as src (for dst)
	//dst.create(src.size(), src.type());

	vector<Vec4i> hierarchy;
	vector<vector<Point> > contours;
	/// Convert the image to grayscale
	resize(src, src, Size (350, 500));
	cvtColor(src, src_gray, CV_BGR2GRAY);
	blur(src_gray, detected_edges, Size(3, 3));
	findContours(detected_edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	imshow("edges", detected_edges);

	///// Show the image
	//CannyThreshold(0, 0);


	/// Wait until user exit program by pressing a key
	waitKey(0);

	return 0;
}