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
#include "simplelinear.hpp"

std::ifstream infile("data.txt");

double sd;

int findPeaks(double arr[], int n)
{
    double sensitivity = sqrt(sd);
    int count = 0;
    //std::cout << "\nsd: " << sd <<"\n";
    
    for (int i = 0; i < (n - 1); i++) {
        if (i==0) continue;
        
        if ((fabs(arr[i] - arr[i-1]) > sensitivity) && (fabs(arr[i]-arr[i+1]) > sensitivity)) {
            std::cout << "max at ";
            std::cout << i << "\n";
            count++;
        }
    }
    return count;
}

double getTempo() {
    int x,y;
    int n = 2;
    double fps = 24.2;
    double a[n],b[n];
    //MotionTrack();
    double slopes[sizeof(infile)*10];
    
    int index = 0;
    int count = 1;
    int j=0;
    
    while (infile >> x >> y)
    {
        a[index]=x;
        b[index]=y;
        
        if (count%n==0) {
            double slope = linSlope(a,b,n);
            slopes[j]=slope;
            //std::cout << slope << "\n";
            j++;
        }
        count++;
    }
    
    sd = calcSD(slopes,n);
    double peaks = findPeaks(slopes,j);
    std::cout << "Number of peaks: " << peaks << "\n";
    std::cout << "in " << count << " data points.\n";
    std::cout << "This means " << peaks/count <<" peaks per data point\n";
    std::cout << "And " << peaks/count*fps <<" peaks per second\n";
    std::cout << "And " << peaks/count*fps*60.0 <<" beats per minute\n";
    
    return 0;
}
