#include "stdafx.h"
#include "FindLine.h"

Mat src_resize, src_gray;
Mat dst0, dst1, cdst, pdst, dst2;

int edgeThresh = 1;
int lowThreshold = 1;
int maxThreshold = 255;

int ratio = 3;
int kernel_size = 3;

vector<int> use_houghLineTransform(Mat src, Mat org)
{
    if (!src.data)
    {
        vector<int> ret = {-1, -1};
        return ret;
    }
    
   // resize(src, src, Size(350,500));
    //blur(src, src, Size(4, 4));
    
    Mat element = getStructuringElement(MORPH_RECT, Size(15, 15), Point(1, 1) );
    
    erode(src,dst0,element);
    
    imshow("eroded",dst0);
    
    //Canny Edge Detector
    Canny(dst0, dst1, 50, 200, 3);
	//cvtColor(dst, cdst, CV_GRAY2BGR);

    vector<Vec4i> lines;
    HoughLinesP(dst1, lines, 1, CV_PI / 180, 50, 50, 10);
    

    vector<int> max_cord = {10000, 10000};

    for (size_t i = 0; i < lines.size(); i++)
    {
        Vec4i l = lines[i];
        line(org, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
       
        // In an image as we move down, y increases but visually we are looking for the highest y value
        if (l[3]<max_cord[1] && l[3]<l[1])
        {
            max_cord[0] = l[2];
            max_cord[1] = l[3];
        }
        else if (l[1]<max_cord[1] && l[1]<l[3])
        {
            max_cord[0] = l[0];
            max_cord[1] = l[1];
        }
    }
    
    circle(org,Point(max_cord[0],max_cord[1]),10,Scalar(0,255,0),2);
    
    imshow("Prob Hough Line Transform", org);
    return max_cord;
}

/** contours -- not used */
int use_contours(Mat src)
{
	if (!src.data || src.empty())
	{
		return -1;
	}

	vector<Vec4i> hierarchy;
	vector<vector<Point> > contours;
	/// Convert the image to grayscale
	resize(src, src, Size (350, 500));

	//cvtColor(src, src_gray, CV_BGR2GRAY);
	blur(src, src_gray, Size(3, 3));
	findContours(src_gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));


	imshow("contoured", src_gray);

    return 0;

}
