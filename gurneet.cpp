#include "stdafx.h"
#include "gurneet.h"

Mat src;
Mat src_resize, src_gray;
Mat dst, cdst, pdst, dst2;

int edgeThresh = 1;
int lowThreshold = 1;
int maxThreshold = 255;
//int const maxThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";


/**
* @function CannyThreshold
* @brief Trackbar callback - Canny thresholds input with a ratio 1:3
*/
void HoughLineTransform()
{
	Canny(src, dst, 50, 200, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);

	//vector<Vec2f> lines;
	//HoughLines(dst, lines, 1, CV_PI / 180, 100, 0, 0);

	//for (size_t i = 0; i < lines.size(); i++)
	//{
	//	float rho = lines[i][0], theta = lines[i][1];
	//	Point pt1, pt2;
	//	double a = cos(theta), b = sin(theta);
	//	double x0 = a*rho, y0 = b*rho;
	//	pt1.x = cvRound(x0 + 1000 * (-b));
	//	pt1.y = cvRound(y0 + 1000 * (a));
	//	pt2.x = cvRound(x0 - 1000 * (-b));
	//	pt2.y = cvRound(y0 - 1000 * (a));
	//	line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
	//}

	//imshow("Hough Line Transform", cdst);

 // probabalistic 
 vector<Vec4i> lines;
 HoughLinesP(dst, lines, 1, CV_PI / 180, 50, 50, 10);
 for (size_t i = 0; i < lines.size(); i++)
 {
 	Vec4i l = lines[i];
 	line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
 }
 imshow("Prob Hough Line Transform", cdst);
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
	blur(src_gray, src_gray, Size(3, 3));
	findContours(src_gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));




	imshow("contoured", src_gray);

	///// Show the image
	HoughLineTransform();


	/// Wait until user exit program by pressing a key
	waitKey(0);

	return 0;
}
