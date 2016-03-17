//
//  MotionTracking.cpp
//  TempoDetection

//
//

//  Copyright © 2015 Gurneet Arora. All rights reserved.
//

#include "GetBinaryImg.hpp"
extern int currentBpm;

// Keep this consistent with getTempo.cpp
const int QUEUESIZE = 60;
const int RECALSIZE = 5;

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
    
    time_t now = time(0);
    
// convert now to string form
     string dt = ctime(&now);
    
        string filename = dt + ".txt";
    
        ofstream datafile;
    
        datafile.open(filename, ios::app);
        datafile.close();
    
    
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
    int tempo = 80;
    
    queue<int> ypos = *new queue<int>;
    int recal_counter = 0;
    
    capture.open(0);

    if(!capture.isOpened()){
        cout<<"ERROR ACQUIRING VIDEO FEED\n";
        getchar();
        return -1;
    }
    
    int last = 0;
    
    bool first = true;
    
    // fps counter begin
    time_t start, end;
    int counter = 0;
    double sec;
    double fps;
    // fps counter end
    
    while(1) {
        // fps counter begin
        if (counter == 0){
            time(&start);
        }
        // fps counter end
        
        if(first){
            //read first frame
            capture.read(frame1);
            //copy second frame
            capture.read(frame2);
            capture.read(frame2);
            first = false;
        }else{
            frame2.copyTo(frame1);
            capture.read(frame2);
            capture.read(frame2);
        }
        //convert frame1 to gray scale for frame differencing
        cv::cvtColor(frame1,grayImage1,COLOR_BGR2GRAY);
        
        //convert frame2 to gray scale for frame differencing
        cv::cvtColor(frame2,grayImage2,COLOR_BGR2GRAY);
        //perform frame differencing with the sequential images. This will output an "intensity image"
        //do not confuse this with a threshold image, we will need to perform thresholding afterwards.
        cv::absdiff(grayImage1,grayImage2,differenceImage);
        
        //blur the image to get rid of the noise. This will output an intensity image
        cv::blur(differenceImage,thresholdImage,cv::Size(BLUR_SIZE,BLUR_SIZE));
        //threshold again to obtain binary image from blur output
        cv::threshold(thresholdImage,thresholdImage,SENSITIVITY_VALUE,255,THRESH_BINARY);

        //gkarora
        vector<int> coord = use_houghLineTransform(thresholdImage, frame1);
        
        if(coord[1] != 10000){
            ypos.push(coord[1]);
            last = coord[1];
        } else {
            ypos.push(last);
        }
        recal_counter++;
        
        //check is queue is full, if so, call getTempo
        // recal_counter needs to be >= for first iteration, recal_counter will be 20
        if (ypos.size() == QUEUESIZE){
            if(recal_counter>=RECALSIZE){
                int oldTempo = tempo;
                tempo = getTempo(ypos, fps);
                double factor = 5;
                if (abs(tempo-oldTempo)< factor)
                {
                    currentBpm = tempo;
                } else if (tempo > (oldTempo+factor)) {
                    currentBpm = oldTempo+factor;
                    tempo = currentBpm;
                } else if (tempo < (oldTempo-factor)) {
                    currentBpm = oldTempo - factor;
                    tempo = currentBpm;
                } else {
                    currentBpm = oldTempo;
                    tempo = currentBpm;
                }
                
                recal_counter =0;
                std::cout << currentBpm << ", ";
                datafile.open(filename, ios::app);
                datafile <<  currentBpm << "\n";
                datafile.close();
                
            }
            ypos.pop();
            
            
        }
        
        ///@MELISSA: CONTROLS for pausing/escape, useful for UI later
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
        
        // fps counter begin
        time(&end);
        counter++;
        sec = difftime(end, start);
        fps = counter/sec*3;
        // overflow protection
        if (counter == (INT_MAX - 1000))
            counter = 0;
        // fps counter end
    }
    //release the capture before re-opening and looping again.
    capture.release();
    
    return 0;
    
}
