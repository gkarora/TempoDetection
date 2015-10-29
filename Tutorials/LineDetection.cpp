//
//  LineDetection.cpp
//  FYDP
//
//  Created by Canmanie Ponnambalam on 2015-10-20.
//  Copyright © 2015 Canmanie Ponnambalam. All rights reserved.
//
//modified from http://docs.opencv.org/doc/tutorials/imgproc/imgtrans/hough_lines/hough_lines.html

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include "LineDetection.h"

using namespace cv;
using namespace std;

void help()
{
    cout << "\nThis program demonstrates line finding with the Hough transform.\n"
    "Usage:\n"
    "./houghlines <image_name>, Default is pic1.jpg\n" << endl;
}

int LineDetection(int argc, char* argv[])
{
    VideoCapture cap("/Users/canmanie/Desktop/4A/461/Videos/Conducting_44_80bpm_Natural.mov"); // open the video file for reading

    if ( !cap.isOpened() )  // if not success, exit program
    {
        cout << "Cannot open the video file" << endl;
        return -1;
    }

    namedWindow("Show Lines",CV_WINDOW_AUTOSIZE); //create a window called "Show Lines"

    // Setup output video
    cv::VideoWriter output_cap(argv[2],
                               cap.get(CV_CAP_PROP_FOURCC),
                               cap.get(CV_CAP_PROP_FPS),
                               cv::Size(cap.get(CV_CAP_PROP_FRAME_WIDTH),
                                        cap.get(CV_CAP_PROP_FRAME_HEIGHT)));

    while(1)
    {
        Mat frame;

        bool bSuccess = cap.read(frame); // read a new frame from video

        if (!bSuccess) {
            cout << "Cannot read the frame from video file" << endl;
            break;
        }

        Mat dst, cdst;
        Canny(frame, dst, 50, 200, 3);
        cvtColor(dst, cdst, CV_GRAY2BGR);

        #if 0
        vector<Vec2f> lines;
        HoughLines(dst, lines, 1, CV_PI/180, 100, 0, 0 );

        for( size_t i = 0; i < lines.size(); i++ )
        {
            float rho = lines[i][0], theta = lines[i][1];
            Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
        }
        #else
        vector<Vec4i> lines;
        HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );
        for( size_t i = 0; i < lines.size(); i++ )
        {
            Vec4i l = lines[i];
            line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
        }
        #endif
        imshow("source", frame);
        imshow("detected lines", cdst);

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }

        output_cap.write(frame);
    }

    //cap.release();
    //output_cap.release();

    return 0;

}
