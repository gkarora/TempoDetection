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

std::ifstream infile("/Users/canmanie/Desktop/4A/461/Data/colourdata44-better.txt");

double fps = 30.0;
double sd;
//double den = 4.0/4.0; //the denominator is the numerator of the time signature

int findPeaks(double arr[], int j)
{
    double sensitivity = sd/10.0;
    int count = 0;
    
    for (int i = 0; i < (j - 1); i++) {
        if (i==0) continue;
        
        if ((arr[i] - arr[i-1] > sensitivity) && (arr[i]-arr[i+1] > sensitivity)) {
            count++;
        }
    }
    return count;
}

double getTempo() {
    int x,y;
    int n = 3;
    double a[n],b[n];
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
            j++;
        }
        count++;
    }
    
    sd = calcSD(slopes,j);
    double peaks = findPeaks(slopes,j);
    peaks = peaks;///den;
    std::cout << "\nsd: " << sd <<"\n";
    std::cout << "Number of peaks: " << peaks << "\n";
    std::cout << "in " << count << " data points.\n";
    std::cout << "This means " << peaks/count <<" peaks per data point\n";
    std::cout << "And " << peaks/count*fps <<" peaks per second\n";
    std::cout << "And " << peaks/count*fps*60.0 <<" beats per minute\n";
    
    return 0;
}
