//
//  simplelinear.cpp
//  FYDP
//
//  Created by Canmanie Ponnambalam on 2015-11-24.
//  Copyright © 2015 Canmanie Ponnambalam. All rights reserved.
//

#include "simplelinear.hpp"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cmath>

using namespace std;

double calcSD(double slopes[], int j){
    double sd = 0;
    double mean = 0;
    double sum = 0;
    
    for (int i = 0; i<j; i++) {
        sum+=slopes[i];
    }
    
    mean = sum/j;
    
    for (int i = 0; i<j; i++) {
        sd += pow((slopes[i]-mean),2);
    }
    sd = sqrt(sd/(double(j)-1));
    
    return sd;
//    cout << "\n" << "standard deviation = ";
//    cout << sd;
}

double linSlope(queue<int> x, queue<int> y, int n)
{
    double intersect = 0;
    double slope = 0;
    
    double sumx = 0;
    double sumy = 0;
    double sumxsquared = 0;
    double sumxy = 0;
    double this_x;
    double this_y;
    
    while (x.size()>0) {
        this_x = x.front();
        this_y = y.front();
        sumx+=this_x;
        sumy+=this_y;
        sumxsquared+=pow(this_x,2);
        sumxy+=this_x*this_y;
        x.pop();
        y.pop();
    }

    slope = ((n*sumxy)-(sumx*sumy))/((n*sumxsquared)-pow(sumx,2));
    intersect = ((sumy*sumxsquared)-sumx*sumxy)/((n*sumxsquared)-pow(sumx,2));

    return slope; //return slope
}