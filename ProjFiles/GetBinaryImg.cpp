//
//  MotionTracking.cpp
//  TempoDetection
//
//  Created by Gurneet Arora on 2015-11-26.
//  Copyright © 2015 Gurneet Arora. All rights reserved.
//

#include "GetBinaryImg.hpp"


//our sensitivity value to be used in the absdiff() function
const static int SENSITIVITY_VALUE = 20;
//size of blur used to smooth the intensity image output from absdiff() function
const static int BLUR_SIZE = 10;
string intToString(int number)
{
    //this function has a number input and string output
    std::stringstream ss;
    ss << number;
    return ss.str();
}

int get_binary_image(){

    bool pause = false;
    //set up the matrices that we will need
    //the two frames we will be comparing
    Mat frame1,frame2;
    //their grayscale images (needed for absdiff() function)
    Mat grayImage1,grayImage2;
    //resulting difference image
    Mat differenceImage;
    //thresholded difference image (for use in findContours() function)
    Mat thresholdImage;
    //video capture object.
    VideoCapture capture;
    
    while(1){
        
        //we can loop the video by re-opening the capture every time the video reaches its last frame
        
        capture.open("test_1.mov");
        
        if(!capture.isOpened()){
            cout<<"ERROR ACQUIRING VIDEO FEED\n";
            getchar();
            return -1;
        }

        ofstream myfile;
        myfile.open ("data.txt", ios::trunc);
        myfile.close();
        
        
        while(capture.get(CV_CAP_PROP_POS_FRAMES)<capture.get(CV_CAP_PROP_FRAME_COUNT)-1){
            
            //read first frame
            capture.read(frame1);
            //convert frame1 to gray scale for frame differencing
            cv::cvtColor(frame1,grayImage1,COLOR_BGR2GRAY);
            //copy second frame
            capture.read(frame2);
            //convert frame2 to gray scale for frame differencing
            cv::cvtColor(frame2,grayImage2,COLOR_BGR2GRAY);
            //perform frame differencing with the sequential images. This will output an "intensity image"
            //do not confuse this with a threshold image, we will need to perform thresholding afterwards.
            cv::absdiff(grayImage1,grayImage2,differenceImage);
            //threshold intensity image at a given sensitivity value
            cv::threshold(differenceImage,thresholdImage,SENSITIVITY_VALUE,255,THRESH_BINARY);
            ////show the difference image and threshold image
            //cv::imshow("Difference Image",differenceImage);
            //cv::imshow("Threshold Image", thresholdImage);

            //blur the image to get rid of the noise. This will output an intensity image
            blur(thresholdImage,thresholdImage,cv::Size(BLUR_SIZE,BLUR_SIZE));
            //threshold again to obtain binary image from blur output
            threshold(thresholdImage,thresholdImage,SENSITIVITY_VALUE,255,THRESH_BINARY);

            //use_contours(thresholdImage);
            vector<int> coord = use_houghLineTransform(thresholdImage);
            if(coord[0] != 10000){
                myfile.open("data.txt", ios::app);
                myfile << intToString(coord[0])+ " "+ intToString(coord[1])+"\n";
                myfile.close();
            }
            //show our captured frame
            //imshow("Frame1",frame1);
            //check to see if a button has been pressed.
            //this 10ms delay is necessary for proper operation of this program
            //if removed, frames will not have enough time to referesh and a blank
            //image will appear.
            switch(waitKey(10)){
                    
                case 27: //'esc' key has been pressed, exit program.
                    return 0;

                case 112: //'p' has been pressed. this will pause/resume the code.
                    pause = !pause;
                    if(pause == true){ cout<<"Code paused, press 'p' again to resume"<<endl;
                        while (pause == true){
                            //stay in this loop until 
                            switch (waitKey()){
                                    //a switch statement inside a switch statement? Mind blown.
                                case 112: 
                                    //change pause back to false
                                    pause = false;
                                    cout<<"Code Resumed"<<endl;
                                    break;
                            }
                        }
                    }
                    
                    
                    
            }
        }
        //release the capture before re-opening and looping again.
        capture.release();
    }
    
    return 0;
    
}