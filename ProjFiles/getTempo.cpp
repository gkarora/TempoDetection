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

std::ifstream infile("data.txt");

double fps = 30.0;
double sd;

int findPeaks(double arr[], int j)
{
    double sensitivity = pow(sd,2);//sqrt(sd);
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

double getTempo() {
    int x,y;
    int n = 5;
    
    std::queue<int> a;
    std::queue<int> b;
    double slopes[sizeof(infile)*10];
    
    int j=0;
    
    while (infile >> x >> y)
    {
        a.push(x);
        b.push(y);
        if (a.size()==n) {
            double slope = linSlope(a,b,n);
            slopes[j]=slope;
            j++;
            a.pop();
            b.pop();
        }
    }
    
    double count = j+4;
    sd = calcSD(slopes,j);
    double peaks = findPeaks(slopes,j);
    std::cout << "Number of peaks: " << peaks << " out of " << j << " points\n";
    std::cout << "in " << count<< " data points.\n";
    std::cout << "This means " << peaks/count <<" peaks per data point\n";
    std::cout << "And " << peaks/count*4*fps <<" peaks per second\n"; //because 4/4 time
    std::cout << "And " << peaks/count*fps*4*60.0 <<" beats per minute\n";
    
    return 0;
}
