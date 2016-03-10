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

//keep consistent with GetBinaryImg.cpp
const int QUEUESIZE = 60;
double sd;

//will take running average of every three
std::vector<int> smoothData(std::queue<int> orig)
{
    std::vector<int> smoothed(QUEUESIZE-2);
    int a=orig.front();
    orig.pop();
    int b=orig.front();
    orig.pop();
    int c = 0;
    int average = 0;
    
    for (int i = 0; i < QUEUESIZE-2; i++) {
        c = orig.front();
        orig.pop();
        
        average = (a+b+c)/3;
        smoothed.at(i)=average;
        a=b;
        b=c;
    }
    
    return smoothed;
}

int findDirectionChanges(std::vector<int> pos)
{
    bool positive = false;
    int count = 0;
    
    for (int i = 0; i < sizeof(pos); i++) {
        
        if (i==0) {
            positive = pos[i]-pos[i-1] > 0;
            continue;
        }
        
        if (positive) {
            if (pos[i]-pos[i-1] < 0) {
                count++;
                positive = false;
            }
        } else {
            if (pos[i]-pos[i-1] > 0) {
                count++;
                positive = true;
            }
        }
    }
    
    return count;
}

int getTempo(std::queue<int> ypos, double fps) {
    std::vector<int> smoothed = smoothData(ypos);
    double peaks = findDirectionChanges(smoothed);
    int tempo = floor((60.0*peaks*fps)/(1.7677*QUEUESIZE));
    return tempo;
}
