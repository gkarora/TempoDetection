// TempoDetection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GetBinaryImg.hpp"
#include "getTempo.hpp"

int main(int argc, const char* argv[])
{
//    Mat src = imread("Capture.PNG");
//	  use_contours(src);
//    use_houghLineTransform(src);
//    waitKey(0);
//    
    get_binary_image();
    getTempo();
	return 0;
}
