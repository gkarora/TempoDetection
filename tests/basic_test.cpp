#include "stdafx.h"

int basic_test(){
	string imageName("test_vid/nyc_test.jpg");
	Mat image;
	image = imread(imageName.c_str(), IMREAD_COLOR); // Read the file

	if (!image.data) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	resize(image, image, Size(image.cols / 4, image.rows / 4));
	namedWindow("Display window", CV_WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display window", image); // Show our image inside it.

	waitKey(0); // Wait for a keystroke in the window
	return 0;
}

int find_hsv(){
	int H_MIN = 0;
	int H_MAX = 256;
	int S_MIN = 0;
	int S_MAX = 256;
	int V_MIN = 0;
	int V_MAX = 256;

	string imageName("test_vid/test_1.jpg");
	Mat image;
	image = imread(imageName.c_str(), IMREAD_COLOR); // Read the file

	if (!image.data) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	Mat HSV;
	Mat threshold;
	cvtColor(image, HSV, COLOR_BGR2HSV);
	//filter HSV image between values and store filtered image to
	//threshold matrix
	inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);
	
	const string windowName = "Original Image";
	const string windowName1 = "HSV Image";
	const string windowName2 = "Thresholded Image";

	imshow(windowName2, threshold);
	imshow(windowName, image);
	imshow(windowName1, HSV);

	waitKey(0); // Wait for a keystroke in the window
	return 0;
}

int find_contour(){
	int thresh = 100;
	RNG rng(12345);
	string imageName("test_vid/test_1.jpg");
	Mat src;
	src = imread(imageName.c_str(), IMREAD_GRAYSCALE);

	if (!src.data){
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	
	namedWindow("Source", CV_WINDOW_AUTOSIZE);
	imshow("Source", src);

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using canny
	Canny(src, canny_output, thresh, thresh * 2, 3);
	/// Find contours
	//findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}

	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);

	waitKey(0);
	return(0);
}