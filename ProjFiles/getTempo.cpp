//
//  getTempo.cpp
//  FYDP
//
//  Created by Canmanie Ponnambalam on 2015-10-20.
//  Copyright © 2015 Canmanie Ponnambalam. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <math.h>
#include <queue>

#include "simplelinear.hpp"

//keep consistent with GetBinaryImg.cpp
const int QUEUESIZE = 60;


std::ifstream infile("colourdata44.txt");
//std::ifstream infile("/Users/canmanie/Desktop/SYDE/4A/461/Data/11data.txt");

double fps = 30.0;
double sd;

int findPeaks(double arr[], int j)
{
    double sensitivity = sd;
    std::cout << "sd: " << sd << "\n";
    int count = 0;
    
    for (int i = 0; i < (j - 1); i++) {
        if (i==0) continue;
        
        if (((arr[i] - arr[i-1] > sensitivity) && (arr[i]-arr[i+1] > sensitivity))) {
            count++;
        }
    }
    return count;
}

int findDirectionChanges(std::queue<int> ypos)
{
    bool pos = false;
    int count = 0;
    int popped;
    int peaked;
    
    for (int i = 0; i < QUEUESIZE-1; i++) {
        
        popped = ypos.front();
        ypos.pop();
        peaked = ypos.front();
        
        if (i==0) continue;
        
        if (pos) {
            if (popped-peaked < 0) {
                count++;
                pos = false;
            }
        } else {
            if (popped-peaked> 0) {
                count++;
                pos = true;
            }
        }
    }
    
    return count;
}

int getTempo(std::queue<int> ypos) {
    
    double peaks = findDirectionChanges(ypos);
    int tempo = floor((60.0*peaks*fps)/(2*QUEUESIZE));
    std::cout << "Number of peaks and troughs: " << peaks << " out of " << QUEUESIZE << " points\n";
    std::cout << "Tempo: " << tempo << "\n" ;
    //one data point per frame at 30 frames per second, 428/30 = 14.267 seconds long
    //80 bpm = 1.3333 beats per second
    //expecting to find
    
    return tempo;
}
